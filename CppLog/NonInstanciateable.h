#ifndef NONINSTANCIATEABLE_H_INCLUDED__
#define NONINSTANCIATEABLE_H_INCLUDED__

/** @brief A class which derives this class protectedly cannot be instanciated. */
class NonInstanciateable
{
protected:
    NonInstanciateable() {}
	NonInstanciateable(const NonInstanciateable&);
	NonInstanciateable& operator= (const NonInstanciateable&);
};

#endif