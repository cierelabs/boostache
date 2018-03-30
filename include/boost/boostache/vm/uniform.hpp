/**
 *  \file generate.hpp
 *
 *  Copyright 2017 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_UNIFORM_HPP
#define BOOST_BOOSTACHE_VM_UNIFORM_HPP

#include <type_traits>
#include <boost/mpl/set.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/boostache/vm/detail/engine_visitor.hpp>
#include <boost/variant/variant.hpp>



namespace boost {
	namespace boostache {
		namespace extension
		{
			template <typename T, typename Enable = void>
			struct context_category
				: mpl::identity<plain_attribute> {};

			template <>
			struct context_category<std::string>
				: mpl::identity<plain_attribute> {};

			template <typename T>
			struct context_category< T
				, vm::trait::enable_if_is_variant_t<T> >
				: mpl::identity<variant_attribute> {};

			template <typename T>
			struct context_category<boost::optional<T>>
				: mpl::identity<optional_attribute> {};


			template <typename T>
			struct context_category<std::map<std::string, T>>
				: mpl::identity<associative_attribute> {};

			template <typename T>
			struct context_category< T
				, vm::trait::enable_if_sequence_not_map_t<T>
			>
				: mpl::identity<sequence_attribute> {};

			//template <typename T1, typename T2>
			//struct context_category<vm::detail::stacked_context<T1, T2>>
			//	: mpl::identity<stacked_context_attribute> {};

			//template <typename T, typename Stream>
			//struct context_category<vm::detail::multi_context<T, Stream>>
			//	: mpl::identity<multi_context_attribute> {};


			template <typename T>
			using context_category_t = typename context_category<T>::type;

		}
	}
}


namespace boost { namespace boostache { namespace vm
{
	namespace detail {


		template <typename TList, typename T> struct ExtendTList;
		template<typename T>
		struct ExtendTList<boost::variant<void>, T> {
			using type = boost::variant<T>;
		};
		template<typename T, typename... Ts>
		struct ExtendTList<boost::variant<Ts...>, T> {
			using type = boost::variant<Ts..., T>;
		};

		template<class Seq>
		using make_simple_variant_over = typename boost::mpl::fold<
			typename boost::mpl::fold<
			Seq,
			boost::mpl::set<>,
			boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
			>::type,
			boost::variant<void>,
			ExtendTList<boost::mpl::_1, boost::mpl::_2>
		>;

		template<class Set>
		using make_simple_variant_over_set = typename boost::mpl::fold<
			Set,
			boost::variant<void>,
			ExtendTList<boost::mpl::_1, boost::mpl::_2>
		>;


		template<typename Data>
		struct collect_data_t;

		template<typename Data, typename Enable = void>
		struct collect_inner_data_t {
			using type = boost::mpl::set<>;
		};

		// specialization for std::string to avoid decomposition of string as sequence of chars
		template<>
		struct collect_inner_data_t<std::string> {
			using type = boost::mpl::set<>;
		};

		template<typename Data>
		struct collect_inner_data_t<boost::optional<Data>> {
			using type = typename collect_data_t<Data>::type;
		};

		template<typename Data>
		struct collect_inner_data_t<std::map<std::string, Data>> {
			using type = typename collect_data_t<Data>::type;
		};

		using std::begin;

		template<typename Data>
		struct collect_inner_data_t<Data, vm::trait::enable_if_sequence_not_map_t<Data>> {


			using value_type = typename std::remove_reference<
				decltype(*begin(std::declval<
					typename std::add_lvalue_reference<Data>::type>()))>::type;

			using type = typename collect_data_t<value_type>::type;
		};

		//template <typename T0, typename... TN>
		//struct collect_inner_data_t<boost::variant<T0, TN...>> {

		//	using inner_datas = boost::mpl::vector<
		//		typename collect_data_t<T0>::type,
		//		typename collect_data_t<TN>::type...>;

		//	using type = typename boost::mpl::fold<
		//		inner_datas
		//		, boost::mpl::set<>
		//		, boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
		//		>::type;
		//};

		template<typename Data>
		struct collect_inner_data_t<Data, vm::trait::enable_if_is_variant_t<Data>> {

			using inner_datas = typename Data::types;

			using type = typename boost::mpl::fold<
				inner_datas
				, boost::mpl::set<>
				, boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
			>::type;
		};



		template<typename Data>
		struct collect_data_t {
			using data_t = std::decay_t<Data>;

			using type = typename boost::mpl::insert<
				typename collect_inner_data_t<data_t>::type,
				data_t
			>::type;
		};













		//template<typename Data>
		//struct collect_leaf_data_t;

        template<typename Data, typename Enable = void>
        struct collect_inner_leaf_data_t;

        template<typename Data>
        struct collect_leaf_data_t {
            using data_t = std::decay_t<Data>;

            using type = typename collect_inner_leaf_data_t<data_t>::type;
        };


		template<typename Data, typename Enable /*= void*/>
		struct collect_inner_leaf_data_t {
			using type = boost::mpl::set<Data>;
		};

		// specialization for std::string to avoid decomposition of string as sequence of chars
		template<>
		struct collect_inner_leaf_data_t<std::string> {
			using type = boost::mpl::set<std::string>;
		};

		template<typename Data>
		struct collect_inner_leaf_data_t<boost::optional<Data>> {
			using type = typename collect_leaf_data_t<Data>::type;
		};

		template<typename Data>
		struct collect_inner_leaf_data_t<std::map<std::string, Data>> {
			using type = typename collect_leaf_data_t<Data>::type;
		};

		using std::begin;

		template<typename Data>
		struct collect_inner_leaf_data_t<Data, vm::trait::enable_if_sequence_not_map_t<Data>> {


			using value_type = typename std::remove_reference<
				decltype(*begin(std::declval<
					typename std::add_lvalue_reference<Data>::type>()))>::type;

			using type = typename collect_leaf_data_t<value_type>::type;
		};



		// joins two sequences
		struct join
		{
			template <class Seq1, class Seq2>
			struct apply
			{
				typedef typename boost::mpl::copy<
					Seq2,
					boost::mpl::back_inserter<Seq1>
				>::type type;
			};
		};

		template<class Sequence>
		struct flatten
		{
			typedef typename boost::mpl::fold<
				Sequence,
				typename boost::mpl::clear<Sequence>::type,
				join
			>::type type;
		};

		template<typename Data>
		struct collect_inner_leaf_data_t<Data, vm::trait::enable_if_is_variant_t<Data>> {

			using inner_datas = typename Data::types;
			using leaf_datas = typename boost::mpl::transform<
				inner_datas,
				collect_leaf_data_t<boost::mpl::_1>
			>::type;

			using flattened = typename flatten<leaf_datas>::type;

			using type = typename boost::mpl::fold<
				flattened
				, boost::mpl::set<>
				, boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
			>::type;
		};



		//template<typename Data>
		//struct collect_leaf_data_t {
		//	using data_t = std::decay_t<Data>;

		//	using type = typename collect_inner_leaf_data_t<data_t>::type;
		//};











		struct tag_empty {};


		template<typename Data>
		struct make_uniform_data_t {

			// mpl-set containing all types from Data
//			using data_types = typename collect_data_t<Data>::type;
			using data_types = typename collect_leaf_data_t<Data>::type;

			// convert set to vector
			using vectorized = typename boost::mpl::copy<
				data_types
				, boost::mpl::back_inserter< boost::mpl::vector<> >
			>::type;

			using data_types_cr = typename boost::mpl::transform<
				vectorized,
				boost::mpl::_1
//				std::add_lvalue_reference<std::add_const<boost::mpl::_1>>
			>::type;

			//static_assert(std::is_reference<typename boost::mpl::at_c<data_types_cr, 0>::type>::value, "ksdhfj");


			using variant_recursive_t = boost::mpl::vector<
				tag_empty,
				boost::optional<boost::recursive_variant_>,
				std::vector<boost::recursive_variant_>,
				std::map<std::string, boost::recursive_variant_>
			>;

			using data_types_cr_recursive = typename boost::mpl::copy<
				data_types_cr
				, boost::mpl::back_inserter<variant_recursive_t>
			>::type;

			using type = typename boost::make_recursive_variant_over<
				data_types_cr_recursive
			>::type;

			//using type = typename detail::make_simple_variant_over_set<
			//	data_types_cr
			//>::type;
		};

	}


	template <typename UniformData, typename Context>
	UniformData transfer_context(Context&& context
		, extension::plain_attribute
	) {
		return context;
	}


	template <typename UniformData, typename Context>
	UniformData transfer_context(Context&& context
		, extension::variant_attribute
	) {
		return boost::apply_visitor(
			//boostache::detail::make_unwrap_variant_visitor(
			[](auto&& ctx)
			{
				return transfer_context<UniformData>(ctx, extension::context_category_t<std::decay_t<decltype(ctx)>>{});
			}
		//)
		, context);
	}





	//template <typename UniformData>
	//UniformData transfer_context(std::string const& context
	//	, plain_attribute
	//) {
	//	return context;
	//}

	template <typename UniformData, typename Data>
	UniformData transfer_context(std::map<std::string, Data> const& context
	, extension::associative_attribute
	) {
		auto m = std::map<std::string, UniformData>();
		for (auto&& pair : context) {
			m[pair.first] = transfer_context<UniformData>(pair.second, extension::context_category_t<Data>{});
		}
		return m;
	}

	template <typename UniformData, typename Data>
	UniformData transfer_context(Data const& context
		, extension::sequence_attribute
	) {
		auto m = std::vector<UniformData>();
		for (auto&& elem : context) {
			m.emplace_back(transfer_context<UniformData>(elem, extension::context_category_t<std::decay_t<decltype(elem)>>{}));
		}
		return m;
	}



   template <typename Context>
   decltype(auto) uniform(Context const& context)
   {
	   using uniform_data_t = typename detail::make_uniform_data_t<Context>::type;
//	   using uniform_tree_t = typename detail::make_uniform_tree_t<Context, uniform_tree_t>::type;
	   //	   uniform_data_t ud(context);
	   //std::string test;
	   //
	   //uniform_data_t ud = transfer_context<uniform_data_t>(context, 
		  // extension::context_category_t<Context>{});

       uniform_data_t udt;
	   return context;
   }
}}}

#endif
