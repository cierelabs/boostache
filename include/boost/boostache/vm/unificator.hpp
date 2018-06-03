/**
 *  \file generate.hpp
 *
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_UNIFORM_HPP
#define BOOST_BOOSTACHE_VM_UNIFORM_HPP

#include <type_traits>
#include <boost/mpl/set.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/boostache/vm/detail/engine_visitor.hpp>
#include <boost/variant/variant.hpp>



namespace boost { namespace boostache { namespace vm
{
	namespace detail {

// gather_types gathers all types from a recursive structure into a set

template <typename T, typename Gathered, bool already_seen, typename Enable = void>
struct gather_types {
    using type = boost::mpl::set<T>;
};

template <typename T, typename Gathered, typename Enable>
struct gather_types<T, Gathered, true, Enable> {
    using type = boost::mpl::set<>;
};


template <typename Gathered>
struct gather_types<std::string, Gathered, false> {
    using type = boost::mpl::set<std::string>;
};



template<typename Gathered>
struct gather_types_helper {

    template<typename T>
    struct apply{
        static constexpr bool already_seen = boost::mpl::has_key<Gathered, T>::type::value;
        using type = typename gather_types<T, Gathered, already_seen>::type;
    };

};


struct append_set {

    template<typename S, typename T>
    struct apply {
        using type = typename boost::mpl::fold<
            T
            , S
            , boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
        >::type;
    };

};


template <typename T, typename Gathered>
struct gather_types< T, Gathered, false
    , vm::trait::enable_if_is_variant_t<T>
> {
    using GatheredT = typename boost::mpl::insert<
        Gathered,
        T
    >::type;

    using data_type_sets = typename boost::mpl::transform<
        typename T::types,
        gather_types_helper<GatheredT>
        >::type;


    using type = typename boost::mpl::fold<
        data_type_sets
        , boost::mpl::set<T>
        , append_set
    >::type;
};

template <typename T, typename Gathered>
struct gather_types< T, Gathered, false
    , vm::trait::enable_if_sequence_not_map_t<T>
> {
    using GatheredT = typename boost::mpl::insert<
        Gathered,
        T
    >::type;


    using value_type = typename std::remove_const<
        typename std::remove_reference<
        decltype(*std::begin(std::declval<T>()))>::type>::type;

    static constexpr bool already_seen = boost::mpl::has_key<GatheredT, value_type>::type::value;

    using base_types = typename gather_types<value_type, GatheredT, already_seen>::type;

    using type = typename boost::mpl::insert<
        base_types,
        T
    >::type;
};

template <typename T, typename Gathered>
struct gather_types<boost::optional<T>, Gathered, false> {
    using GatheredT = typename boost::mpl::insert<
        Gathered,
        boost::optional<T>
    >::type;

    static constexpr bool already_seen = boost::mpl::has_key<GatheredT, T>::type::value;
    using base_types = typename gather_types<T, GatheredT, already_seen>::type;

    using type = typename boost::mpl::insert<
        base_types,
        boost::optional<T>
    >::type;
};

template <typename T, typename Gathered>
struct gather_types<std::map<std::string, T>, Gathered, false> {
    using GatheredT = typename boost::mpl::insert<
        Gathered,
        std::map<std::string, T>
    >::type;

    static constexpr bool already_seen = boost::mpl::has_key<GatheredT, T>::type::value;
    using base_types = typename gather_types<T, GatheredT, already_seen>::type;

    using type = typename boost::mpl::insert<
        base_types,
        std::map<std::string, T>
    >::type;
};




        //// foreach category
        //template <typename T, typename Seen, typename Enable = void>
        //struct collect_map_data {
        //    using type = boost::mpl::set<>;
        //};

        //template <typename Seen>
        //struct collect_map_data<std::string, Seen> {
        //    using type = boost::mpl::set<>;
        //};


        //template <typename T, typename Seen>
        //struct lazy_collect_map_data<T, Seen> {
        //    using type = collect_map_data<T, Seen>::type;
        //};

        //template <typename T, typename Seen>
        //struct collect_map_data< T, Seen
        //    , vm::trait::enable_if_is_variant_t<T>
        //> {
        //    using SeenT = typename boost::mpl::insert<
        //        Seen,
        //        T
        //        >::type;

        //    using data_types = typename boost::mpl::transform<
        //        typename T::types,
        //        //				boost::mpl::_1

        //        boost::mpl::eval_if<
        //         typename boost::mpl::has_key<SeenT, boost::mpl::_1>::type
        //        , collect_map_data<void, SeenT>
        //        , lazy_collect_map_data<boost::mpl::_1, SeenT>
        //        >::type;

        //    using type = typename boost::mpl::fold<
        //        data_types
        //        , boost::mpl::set<>
        //        , boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
        //    >::type;
        //};

        //template <typename T, typename Seen>
        //struct collect_map_data< T, Seen
        //    , vm::trait::enable_if_sequence_not_map_t<T>
        //> {
        //    using SeenT = typename boost::mpl::insert<
        //        Seen,
        //        T
        //    >::type;

        //    using value_type = typename std::remove_const<
        //        typename std::remove_reference<
        //        decltype(*std::begin(std::declval<T>()))>::type>::type;

        //    using type = typename collect_map_data<value_type, SeenT>::type;
        //};

        //template <typename T, typename Seen>
        //struct collect_map_data<boost::optional<T>, Seen> {
        //    using SeenT = typename boost::mpl::insert<
        //        Seen,
        //        boost::optional<T>
        //    >::type;
        //    using type = typename collect_map_data<T, SeenT>::type;
        //};

        //template <typename T, typename Seen>
        //struct collect_map_data<std::map<std::string, T>, Seen> {
        //    using SeenT = typename boost::mpl::insert<
        //        Seen,
        //        std::map<std::string, T>
        //    >::type;

        //    using base_types = typename collect_map_data<T, SeenT>::type;

        //    using type = typename boost::mpl::insert<
        //        base_types,
        //        std::map<std::string, T>
        //        >::type;
        //};


template <typename T>
struct is_map_data : public boost::mpl::false_ {};

template <typename T>
struct is_map_data<std::map<std::string, T>> : public boost::mpl::true_ {};

		struct tag_empty {};


		template<typename Data>
		struct make_uniform_data_t {

			// mpl-set containing all map-types from Data
            using data_types = typename gather_types<Data, boost::mpl::set<>, false>::type;
            //using map_data_types = typename collect_map_data<Data, boost::mpl::set<>>::type;

			// convert set to vector
            using vectorized = typename boost::mpl::copy<
                data_types
                , boost::mpl::back_inserter< boost::mpl::vector<> >
            >::type;

            // filter out non-maps
            //using vectorized_map_data = typename boost::mpl::fold<
            //    vectorized
            //    , boost::mpl::vector<>
            //    , boost::mpl::if_< is_map_data<boost::mpl::_2>,
            //    boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>,
            //    boost::mpl::_1
            //        >
            //>::type;

            using vectorized_map_data = vectorized;



            // ensure const&
			using data_types_cr = typename boost::mpl::transform<
                vectorized_map_data,
				std::add_lvalue_reference<std::add_const<std::remove_reference<boost::mpl::_1>>>
			>::type;

            using type = typename boost::make_variant_over<data_types_cr>::type;
		};

	}

    template <typename Context>
    using uniform_data_t = typename detail::make_uniform_data_t<Context>::type;


}}}

#endif
