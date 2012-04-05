#ifndef WIN32_EXCEPTION_H_36B2BFE3_B829_4E85_8D8B_015C50E60A88_INCLUDED__
#define WIN32_EXCEPTION_H_36B2BFE3_B829_4E85_8D8B_015C50E60A88_INCLUDED__

#include <exception>
#include <string>
#include <sstream>

/**
* @brief Class for defining a custom exception.
*
* An instance of this class is responsible to create a message
* string which is used for the base class construction.
*/
class Win32Exception : public std::runtime_error
{
public:
    /**
    * Constructor.
    * @param msg [in] : reference to the message
    * @param code [in] : the error code
    * @param pszFile [in] : pointer to the zero terminated string which contains the file where the exception was raised
    * @param line [in] : line number within the file where the exception was raised
    */
    Win32Exception(std::string& msg, int code, const char* pszFile, int line) :
        std::runtime_error(CreateMessage(msg, code, pszFile, line)), m_Code(code) {}

    /**
    * Constructor.
    * @param pszMsg [in] : pointer to the zero terminated message string
    * @param code [in] : the error code
    * @param pszFile [in] : pointer to the zero terminated string which contains the file where the exception was raised
    * @param line [in] : line number within the file where the exception was raised
    */
    Win32Exception(const char* pszMsg, int code, const char* pszFile, int line) :
        std::runtime_error(CreateMessage(pszMsg, code, pszFile, line)), m_Code(code) {}

    /**
    * The error code; usually this is the native Windows Error code provided by
    * ::GetLastError(); But it also can contain another code giving context information
    * of the error reason.
    */
    int Code() const { return m_Code; }

private:

	/**
	* Utilitiy function for creation of a message string.
	* @param msg [in] : reference to the first part of the resulting message
	* @param code [in] : the code (usually this contains ::GetLastError() value
	* @param pszFile [in] : pointer to the zero terminated string which contains a file name
	* @param line [in] : the line nummer within the source file where the message belongs to
	* @return std::string : the formatted string message
	*/
    static std::string CreateMessage(std::string& msg, int code, const char* pszFile, int line)
    {
        std::stringstream ss;
        ss << msg << ", error " << code << " " << std::string(pszFile) << " (" << line << ")";
        return ss.str();
    }

	/**
	* Utilitiy function for creation of a message string.
	* @param pszMsg [in] : pointer to the zeroterminated message which forms the first part of the resulting message
	* @param code [in] : the code (usually this contains ::GetLastError() value
	* @param pszFile [in] : pointer to the zero terminated string which contains a file name
	* @param line [in] : the line nummer within the source file where the message belongs to
	* @return std::string : the formatted string message
	*/
    static std::string CreateMessage(const char* pszMsg, int code, const char* pszFile, int line)
    {
        std::stringstream ss;
        ss << std::string(pszMsg) << ", error " << code << " " <<  std::string(pszFile) << " (" << line << ")";
        return ss.str();
    }

    int m_Code; ///< the code (usually this is a copy of the ::GetLastError() value)
};

#endif
