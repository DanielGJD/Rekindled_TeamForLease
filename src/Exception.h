/*!
    \file   Exception.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class that all other exception should inherit from

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

namespace ForLeaseEngine {
    /*!
        \class Exception

        \brief
            Contains a message string to be thrown upon error, base class for all exceptions
    */
    class Exception {
        public:
            Exception();
            Exception(const std::string& exceptionMessage);

            void SetInfo(const std::string& exceptionMessage);
            std::string const& GetInfo() const;

        private:
            //! Error message
            std::string message;
    };
}

#endif
