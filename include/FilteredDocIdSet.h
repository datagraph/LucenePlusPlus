/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2010 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocIdSet.h"
#include "FilteredDocIdSetIterator.h"

namespace Lucene
{
	/// Abstract decorator class for a DocIdSet implementation that provides on-demand filtering/validation
	/// mechanism on a given DocIdSet.
	///
	/// Technically, this same functionality could be achieved with ChainedFilter (under contrib/misc), however 
	/// the benefit of this class is it never materializes the full bitset for the filter.  Instead, the {@link 
	/// #match} method is invoked on-demand, per docID visited during searching.  If you know few docIDs will 
	/// be visited, and the logic behind {@link #match} is relatively costly, this may be a better way to filter 
	/// than ChainedFilter.
	/// @see DocIdSet
	class LPPAPI FilteredDocIdSet : public DocIdSet
	{
	public:
		/// @param innerSet Underlying DocIdSet
		FilteredDocIdSet(DocIdSetPtr innerSet);
		virtual ~FilteredDocIdSet();
	
		LUCENE_CLASS(FilteredDocIdSet);
	
	protected:
		DocIdSetPtr innerSet;
	
	public:
		/// This DocIdSet implementation is cacheable if the inner set is cacheable.
		virtual bool isCacheable();
		
		/// Implementation of the contract to build a DocIdSetIterator.
		/// @see DocIdSetIterator
		/// @see FilteredDocIdSetIterator
		virtual DocIdSetIteratorPtr iterator();
		
	protected:
		/// Validation method to determine whether a docid should be in the result set.
		/// @param docid docid to be tested
		/// @return true if input docid should be in the result set, false otherwise.
		virtual bool match(int32_t docid) = 0;
		
		friend class DefaultFilteredDocIdSetIterator;
	};
	
	/// Implementation of the contract to build a DocIdSetIterator.
	class LPPAPI DefaultFilteredDocIdSetIterator : public FilteredDocIdSetIterator
	{
	public:
		DefaultFilteredDocIdSetIterator(FilteredDocIdSetPtr filtered, DocIdSetIteratorPtr innerIter);
		virtual ~DefaultFilteredDocIdSetIterator();
	
		LUCENE_CLASS(DefaultFilteredDocIdSetIterator);
	
	protected:
		FilteredDocIdSetPtr filtered;
	
	protected:
		virtual bool match(int32_t docid);
	};
}
