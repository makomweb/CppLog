#ifndef NONCOPYABLE_H_111F9722_E762_402B_AAF1_FC486BD72532_INCLUDED__
#define NONCOPYABLE_H_111F9722_E762_402B_AAF1_FC486BD72532_INCLUDED__

/** @brief A class which derives this class protectedly cannot be instanciated via copy or assignment. */
class NonCopyable
{
protected:
	NonCopyable() {}
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};

#endif
