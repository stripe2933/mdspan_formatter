#pragma once

#ifdef MDSPAN_FORMATTER_USE_FMT
#include <fmt/ranges.h>
#else
#include <format>
#endif

#include <experimental/mdspan>
#include <experimental/__p2630_bits/submdspan.hpp>

template <typename T, typename Extents, typename LayoutPolicy, typename CharT>
#ifdef MDSPAN_FORMATTER_USE_FMT
class fmt::formatter<std::mdspan<T, Extents, LayoutPolicy>, CharT> : public range_formatter<T, CharT>{
#else
class std::formatter<std::mdspan<T, Extents, LayoutPolicy>, CharT> : public range_formatter<T, CharT>{
#endif
public:
    constexpr auto format(const auto &x, auto &ctx) const{
        return format_submdspan(reduce_dimension(x), ctx, 1);
    }

private:
    constexpr auto format_submdspan(auto &&x, auto &ctx, std::size_t depth) const{
        constexpr auto rank = std::remove_cvref_t<decltype(x)>::rank();

        if constexpr (rank == 1){
            return range_formatter<T, CharT>::format(std::span { x.data_handle(), static_cast<std::size_t>(x.extent(0)) }, ctx);
        }
        else{
            format_to(ctx.out(), "[");

            const auto primary_extent = x.extent(std::is_same_v<LayoutPolicy, std::layout_right> ? 0 : rank - 1);
            for (std::size_t i = 0; i < primary_extent - (std::size_t)1; ++i){
                format_submdspan(reduce_dimension(x, i), ctx, depth + 1);
                format_to(ctx.out(), ",\n{0: >{1}}", "", depth);
            }
            format_submdspan(reduce_dimension(x, primary_extent - (std::size_t)1), ctx, depth + 1);

            return format_to(ctx.out(), "]");
        }
    }

    /**
     * Reduce a mdspan with given primary indices.
     * @param x A source mdspan to be reduced.
     * @param indices Indices of dropping axis.
     * @return Reduced mdspan whose rank is decreased by given number of indices.
     *         The return mdspan will be <tt> x[indices..., full_extent...] </tt> for \p layout_right and
     *         <tt> x[full_extent..., indices...] </tt> for \p layout_left.
     * @example
     * @code
     * auto ints = std::array { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
     *
     * auto int2x2x3 = mdspan { ints.data(), 2, 2, 3 };
     * reduce_dimension(int2x2x3, 0); // -> [[1, 2, 3], [4, 5, 6]]
     * reduce_dimension(int2x2x3, 1, 1); // -> [10, 11, 12]
     *
     * auto int4x3 = mdspan<int, extents<std::size_t, 2, 2, 3>, layout_left> { ints.data() };
     * reduce_dimension(int4x3, 1, 2); // -> [11, 12]
     * @endcode
     */
    template <typename... Indices>
    static constexpr auto reduce_dimension(auto &&x, Indices ...indices) {
        return [&]<std::size_t... I>(std::index_sequence<I...>){
            if constexpr (std::is_same_v<LayoutPolicy, std::layout_right>){
                return std::experimental::submdspan(x, indices..., std::make_pair(I, std::full_extent).second...);
            }
            else{
                return std::experimental::submdspan(x, std::make_pair(I, std::full_extent).second..., indices...);
            }
        }(std::make_index_sequence<std::remove_cvref_t<decltype(x)>::rank() - sizeof...(Indices)>{});
    }
};