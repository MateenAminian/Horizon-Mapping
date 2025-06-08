//
// This file is part of the Terathon Common Library, by Eric Lengyel.
// Copyright 1999-2022, Terathon Software LLC
//
// This software is licensed under the GNU General Public License version 3.
// Separate proprietary licenses are available from Terathon Software.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
// EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//


#ifndef TSCompletable_h
#define TSCompletable_h


//# \component	Utility Library
//# \prefix		Utilities/


#include "TSPlatform.h"


namespace Terathon
{
	//# \class	Completable		The base class for objects that can complete a time-consuming task.
	//
	//# Objects inherit from the $Completable$ class when they perform a time-consuming task
	//# and wish to provide a mechanism for notifying the user of the object when such a task
	//# has been completed.
	//
	//# \def	template <class classType> class Completable
	//
	//# \tparam		classType	The type of the class which performs a completable action. This parameter should be the type of the class that inherits directly from the $Completable$ class.
	//
	//# \ctor	Completable();
	//
	//# \desc
	//# The $Completable$ class encapsulates a completion callback function pointer and a data value that is
	//# passed to this function. The completion callback function is invoked when the $@Completable::HandleCompletion@$
	//# function is called. This usually happens when a subclass of $Completable$ completes a time consuming task
	//# and needs to notify some other piece of code.
	//#
	//# Upon construction, the completion callback function is set to $nullptr$, causing calls to the
	//# $@Completable::HandleCompletion@$ function to have no effect.


	//# \function	Completable::GetCompletionCallback		Returns the completion callback function for an object.
	//
	//# \proto		CompletionCallback *GetCompletionCallback(void) const;
	//
	//# \desc
	//# The $GetCompletionCallback$ function returns a pointer to the currently installed completion callback function.
	//# If no completion callback function has been installed, then this function returns $nullptr$. The $CompletionCallback$
	//# type is defined as
	//
	//# \code	typedef void CompletionCallback(classType *, void *);
	//
	//# where $classType$ is the template parameter for the $Completable$ class template.
	//
	//# \also	$@Completable::SetCompletionCallback@$
	//# \also	$@Completable::GetCompletionCookie@$
	//# \also	$@Completable::SetCompletionCookie@$
	//# \also	$@Completable::HandleCompletion@$


	//# \function	Completable::SetCompletionCallback		Sets the completion callback function for an object.
	//
	//# \proto		void SetCompletionCallback(CompletionCallback *callback);
	//# \proto		void SetCompletionCallback(CompletionCallback *callback, void *cookie);
	//
	//# \param		callback	A pointer to the completion callback function. The function passed will be invoked
	//#							whenever $@Completable::HandleCompletion@$ is called (usually from a subclass).
	//#							The $callback$ parameter may be $nullptr$, in which case the object has no completion
	//#							callback function, and calls to $@Completable::HandleCompletion@$ have no effect.
	//# \param		cookie		The cookie that is passed to the completion callback function as its last parameter.
	//
	//# \desc
	//# The $SetCompletionCallback$ function sets the completion callback function for an object that inherits from
	//# the $@Completable@$ class template. The $CompletionCallback$ type is defined as
	//
	//# \code	typedef void CompletionCallback(classType *, void *);
	//
	//# where $classType$ is the template parameter for the $Completable$ class template. When a subclass
	//# completes a task and needs to notify the user of the object that the task has completed, it calls
	//# the $@Completable::HandleCompletion@$ function.
	//
	//# \also	$@Completable::GetCompletionCallback@$
	//# \also	$@Completable::GetCompletionCookie@$
	//# \also	$@Completable::SetCompletionCookie@$
	//# \also	$@Completable::HandleCompletion@$


	//# \function	Completable::GetCompletionCookie	Returns the cookie that is passed to the completion callback function.
	//
	//# \proto		void *GetCompletionCookie(void) const;
	//
	//# \desc
	//# The $GetCompletionCookie$ function returns the cookie that is passed to the completion callback function.
	//# The cookie is undefined until it is set by calling either the $@Completable::SetCompletionCallback@$ function
	//# or the $@Completable::SetCompletionCookie@$ function.
	//
	//# \also	$@Completable::GetCompletionCallback@$
	//# \also	$@Completable::SetCompletionCallback@$
	//# \also	$@Completable::SetCompletionCookie@$
	//# \also	$@Completable::HandleCompletion@$


	//# \function	Completable::SetCompletionCookie	Sets the cookie that is passed to the completion callback function.
	//
	//# \proto		void SetCompletionCookie(void *cookie);
	//
	//# \param		cookie		The cookie that is passed to the completion callback function as its last parameter.
	//
	//# \desc
	//# The $SetCompletionCookie$ function sets the cookie that is passed to the completion callback function for an object
	//# that inherits from the $@Completable@$ class template. When a subclass completes a task and needs to notify
	//# the user of the object that the task has completed, it calls the $@Completable::HandleCompletion@$ function.
	//
	//# \also	$@Completable::GetCompletionCallback@$
	//# \also	$@Completable::SetCompletionCallback@$
	//# \also	$@Completable::GetCompletionCookie@$
	//# \also	$@Completable::HandleCompletion@$


	//# \function	Completable::HandleCompletion		Invokes the completion callback function for an object.
	//
	//# \proto		void HandleCompletion(void);
	//
	//# \desc
	//# The $HandleCompletion$ function invokes the currently installed completion callback function for an object. If no
	//# completion callback function is installed, then calling the $HandleCompletion$ function has no effect. A completion
	//# callback function is installed by calling the $@Completable::SetCompletionCallback@$ function.
	//
	//# \also	$@Completable::GetCompletionCallback@$
	//# \also	$@Completable::SetCompletionCallback@$
	//# \also	$@Completable::GetCompletionCookie@$
	//# \also	$@Completable::SetCompletionCookie@$


	template <class classType>
	class Completable
	{
		public:

			typedef void CompletionCallback(classType *, void *);

		private:

			CompletionCallback		*completionCallback;
			void					*completionCookie;

		protected:

			Completable()
			{
				completionCallback = nullptr;
			}

		public:

			virtual ~Completable() = default;

			CompletionCallback *GetCompletionCallback(void) const
			{
				return (completionCallback);
			}

			void SetCompletionCallback(CompletionCallback *callback)
			{
				completionCallback = callback;
			}

			void *GetCompletionCookie(void) const
			{
				return (completionCookie);
			}

			void SetCompletionCookie(void *cookie)
			{
				completionCookie = cookie;
			}

			void SetCompletionCallback(CompletionCallback *callback, void *cookie)
			{
				completionCallback = callback;
				completionCookie = cookie;
			}

			void HandleCompletion(void)
			{
				if (completionCallback)
				{
					(*completionCallback)(static_cast<classType *>(this), completionCookie);
				}
			}
	};
}


#endif
