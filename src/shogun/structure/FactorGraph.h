/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2013 Shell Hu 
 * Copyright (C) 2013 Shell Hu 
 */

#ifndef __FACTORGRAPH_H__
#define __FACTORGRAPH_H__

#include <shogun/lib/DynamicObjectArray.h>
#include <shogun/lib/SGVector.h>
#include <shogun/features/Features.h>
#include <shogun/structure/Factor.h>
#include <shogun/structure/FactorGraphLabels.h>

namespace shogun {

/** @brief Class CFactorGraph a factor graph is a structured input in general 
 */
class CFactorGraph : public CFeatures
{
	class CDisjointSet;

public:
	CFactorGraph();

	/** Constructor 
	 *
	 * @param card cardinalities of all the variables in the factor graph 
	 */
	CFactorGraph(const SGVector<int32_t> card);

	/** Copy constructor 
	 *
	 * @param fg a factor graph instance
	 */
	CFactorGraph(const CFactorGraph &fg);

	/** deconstructor */
	~CFactorGraph();

	/** @return class name */
	virtual const char* get_name() const { return "FactorGraph"; }

	/** add a factor 
	 *
	 * @param factor a factor pointer
	 */
	void add_factor(CFactor* factor);

	/** add a data source  
	 *
	 * @param datasource a factor data source 
	 */
	void add_data_source(CFactorDataSource* datasource);

	/** @return all the factors */
	CDynamicObjectArray* get_factors() const;

	/** @return all the shared data */
	CDynamicObjectArray* get_factor_data_sources() const;

	/** @return cardinalities */
	SGVector<int32_t> get_cardinalities() const;

	/** set cardinalities 
	 *
	 * @param cards cardinalities of all variables
	 */
	void set_cardinalities(SGVector<int32_t> cards);

	/** compute energy tables in the factor graph */ 
	void compute_energies();

	/** evaluate energy given full assignment 
	 *
	 * @param state an assignment 
	 */
	float64_t evaluate_energy(const SGVector<int32_t> state) const;

	/** evaluate energy for a given fully observed assignment
	 *
	 * @param obs factor graph observation 
	 */
	float64_t evaluate_energy(const CFactorGraphObservation* obs) const;

	/** @return copy of factor graph */
	virtual CFactorGraph* duplicate() const;

	/** @return feature tyep */
	virtual EFeatureType get_feature_type() const {return F_ANY;}

	/** @return feature class */
	virtual EFeatureClass get_feature_class() const {return C_ANY;}

	/** @return number of factors */
	virtual int32_t get_num_vectors() const;

	/** @return disjoint set */
	CDisjointSet* get_disjoint_set() const;

	/** @return number of edges */
	int32_t get_num_edges() const;

	/** connect graph nodes by performing union-find algorithm */
	void connect_components();

	/** @return is acyclic graph or not */
	bool is_acyclic_graph() const;

	/** @return is connected graph or not */
	bool is_connected_graph() const;

	/** @return is tree graph or not */
	bool is_tree_graph() const;

private:
	/** register parameters */
	void register_parameters();

	/** initialization */
	void init();

protected:
	// TODO: FactorNode, VariableNode, such that they have IDs

	/** cardinalities */
	SGVector<int32_t> m_cardinalities;

	/** added factors */
	CDynamicObjectArray* m_factors;

	/** added data sources */
	CDynamicObjectArray* m_datasources;

	/** disjoint set */
	CDisjointSet* m_dset;

	/** if has circle in the graph */
	bool m_has_cycle;

	/** number of edges */
	int32_t m_num_edges;
	
private:
	/** @brief Class CDisjointSet data structure for linking graph nodes
	 * It's easy to identify connected graph, acyclic graph, roots of forest etc.
	 * please refer to http://en.wikipedia.org/wiki/Disjoint-set_data_structure
	 */
	class CDisjointSet : public CSGObject
	{
	public:
		/** default constructor */
		CDisjointSet();
	
		/** constructor 
		 *
		 * @param num_elements number of initial disjoint elements
		 */
		CDisjointSet(int32_t num_elements);
	
		/** destructor */
		~CDisjointSet() { }
	
		/** @return class name */
		virtual const char* get_name() const { return "DisjointSet"; }
	
		/** initialize internal data structures */
		void make_sets();
	
		/** find root of the set containing x with path compression 
		 *
		 * @param x queried element 
		 * @return the root
		 */
		int32_t find_set(int32_t x);
	
		/** link two roots, higher ranked root will be new root
		 * 
		 * @param xroot root of the set containing x 
		 * @param yroot root of the set containing y 
		 * @return new root 
		 */
		int32_t link_set(int32_t xroot, int32_t yroot);
	
		/** link the roots of two sets containing x and y respectively 
		 * and return if they were linked 
		 *
		 * @param x first element to be linked
		 * @param y second element to be linked
		 * @return if x and y were in the same set
		 */
		bool union_set(int32_t x, int32_t y);
	
		/** if element x and element y is in the same set
		 *
		 * @param x first element
		 * @param y second element
		 * @return if x and y are in the same set
		 */
		bool is_same_set(int32_t x, int32_t y);
	
		/** give each disjoint set a label
		 *
		 * @param out_labels label for each element
		 * @return number of unique labels
		 */
		int32_t get_unique_labeling(SGVector<int32_t> out_labels);
	
		/** get number of sets
		 *
		 * @return number of sets
		 */
		int32_t get_num_sets();
	
		/** if union-find is performed
		 *
		 * @return is connected
		 */
		bool get_connected();
	
		/** set connection flag after union-find
		 *
		 * @param is_connected boolean variable
		 */
		void set_connected(bool is_connected);
	
	private:
		/** register parameters */
		void register_parameters();
	
	private:
		/** number of elements */
		int32_t m_num_elements;
	
		/** parent array */
		SGVector<int32_t> m_parent;
	
		/** rank array */
		SGVector<int32_t> m_rank;
	
		/** connection flag */
		bool m_is_connected;
	};

};

}

#endif

