//
// Created by gomkyung2 on 2023/05/11.
//

#ifndef WCHAR_MDSPAN_FORMATTER_HPP
#define WCHAR_MDSPAN_FORMATTER_HPP

// TODO: mdspan formatter with layout_left policy is not implemented yet.

#include <format>
#include <ranges>
#include <array>

#include <mdspan/mdspan.hpp>

template <typename T, typename Extents, typename CharT>
struct std::formatter<Kokkos::mdspan<T, Extents>, CharT> : range_formatter<remove_cv_t<T>, CharT>{
public:
    template <typename FormatContext>
    constexpr auto format(Kokkos::mdspan<T, Extents> x, FormatContext &ctx) const{
        return recursive_format(x, ctx, 1UZ);
    }

private:
    template <typename InnerExtents, typename FormatContext>
    constexpr auto recursive_format(Kokkos::mdspan<T, InnerExtents> x, FormatContext &ctx, size_t depth) const{
        if constexpr (InnerExtents::rank() == 1UZ){
            // Format 1-dimensional span using specified format.
            const auto span = [x]{
                if constexpr (InnerExtents::static_extent(0) == dynamic_extent){
                    return std::span { x.data_handle(), static_cast<size_t>(x.extent(0)) };
                }
                else{
                    return std::span<T, InnerExtents::static_extent(0)> { x.data_handle(), x.data_handle() + InnerExtents::static_extent(0) };
                }
            }();

            range_formatter<remove_cv_t<T>, CharT>::format(span, ctx);
        }
        else{
            // Get extents<T, I2, ..., IN> from extents<T, I1, I2, ..., IN> (skip head template parameter).
            using subextents = decltype([]<size_t I, size_t... J>(index_sequence<I, J...>){
                return Kokkos::extents<typename InnerExtents::index_type, InnerExtents::static_extent(J)...>{};
            }(make_index_sequence<InnerExtents::rank()>{}));

            // Get number of dynamic extents in subextents.
            constexpr size_t num_dynamic_subextents = []<size_t... I>(index_sequence<I...>){
                return (static_cast<size_t>(subextents::static_extent(I) == dynamic_extent) + ...);
            }(make_index_sequence<subextents::rank()>{});

            // Get x[0, ...], x[1, ...], ..., x[x.extents(0)-1, ...] view.
            const auto subspans = [=](){
                const auto stride = x.stride(0UZ);

                if constexpr (num_dynamic_subextents > 0UZ) {
                    // Since subextents contains dynamic_extent, dynamic extents should be manually passed to
                    // mdspan constructor with fixed-size array type.
                    array<size_t, num_dynamic_subextents> dynamic_subextents;
                    for (auto i = 0UZ, j = 0UZ; j < num_dynamic_subextents; ++i) {
                        if (subextents::static_extent(i) == dynamic_extent) {
                            dynamic_subextents[j++] = x.extent(i + 1);
                        }
                    }

                    return views::iota(static_cast<Extents::index_type>(0), x.extent(0))
                           | views::transform([=](auto idx) { return Kokkos::mdspan<T, subextents> { x.data_handle() + stride * idx, dynamic_subextents }; });
                }
                else{
                    return views::iota(static_cast<Extents::index_type>(0), x.extent(0))
                           | views::transform([=](auto idx) { return Kokkos::mdspan<T, subextents> { x.data_handle() + stride * idx }; });
                }
            }();

            format_to(ctx.out(), "[");
            for (auto &&subspan : subspans | views::take(subspans.size() - 1UZ)){
                recursive_format(subspan, ctx, depth + 1);

                /*
                 * Add left spacing by depth level.
                 * [[1, 2, 3],       [[1, 2, 3],
                 * [4, 5, 6]]    ->   [4, 5, 6]] (improved readability)
                 */
                format_to(ctx.out(), ",\n{0: ^{1}}", "", depth);
            }
            recursive_format(subspans.back(), ctx, depth + 1);
            format_to(ctx.out(), "]");
        }
        return ctx.out();
    }
};

#endif //WCHAR_MDSPAN_FORMATTER_HPP
