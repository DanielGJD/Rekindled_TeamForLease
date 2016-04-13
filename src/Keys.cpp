/*!
    \file   Keys.cpp
    \author Christopher Hudson

    \brief
        Defines a map to translate strings to keycodes

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Keys.h"
#include <iostream>

namespace ForLeaseEngine {
    std::unordered_map<std::string, int> Keys::StringToKeycode = std::unordered_map<std::string, int>();

    void Keys::InitKeymap() {
        StringToKeycode.insert(std::make_pair("One", int(Keys::One)));
        StringToKeycode.insert(std::make_pair("Two", int(Keys::Two)));
        StringToKeycode.insert(std::make_pair("Three", int(Keys::Three)));
        StringToKeycode.insert(std::make_pair("Four", int(Keys::Four)));
        StringToKeycode.insert(std::make_pair("Five", int(Keys::Five)));
        StringToKeycode.insert(std::make_pair("Six", int(Keys::Six)));
        StringToKeycode.insert(std::make_pair("Seven", int(Keys::Seven)));
        StringToKeycode.insert(std::make_pair("Eight", int(Keys::Eight)));
        StringToKeycode.insert(std::make_pair("Nine", int(Keys::Nine)));
        StringToKeycode.insert(std::make_pair("Zero", int(Keys::Zero)));
        StringToKeycode.insert(std::make_pair("NumpadZero", int(Keys::NumpadZero)));
        StringToKeycode.insert(std::make_pair("NumpadOne", int(Keys::NumpadOne)));
        StringToKeycode.insert(std::make_pair("NumpadTwo", int(Keys::NumpadTwo)));
        StringToKeycode.insert(std::make_pair("NumpadThree", int(Keys::NumpadThree)));
        StringToKeycode.insert(std::make_pair("NumpadFour", int(Keys::NumpadFour)));
        StringToKeycode.insert(std::make_pair("NumpadFive", int(Keys::NumpadFive)));
        StringToKeycode.insert(std::make_pair("NumpadSix", int(Keys::NumpadSix)));
        StringToKeycode.insert(std::make_pair("NumpadSeven", int(Keys::NumpadSeven)));
        StringToKeycode.insert(std::make_pair("NumpadEight", int(Keys::NumpadEight)));
        StringToKeycode.insert(std::make_pair("NumpadNine", int(Keys::NumpadNine)));
        StringToKeycode.insert(std::make_pair("NumpadPlus", int(Keys::NumpadPlus)));
        StringToKeycode.insert(std::make_pair("NumpadMultiply", int(Keys::NumpadMultiply)));
        StringToKeycode.insert(std::make_pair("NumpadMinus", int(Keys::NumpadMinus)));
        StringToKeycode.insert(std::make_pair("NumpadDivide", int(Keys::NumpadDivide)));
        StringToKeycode.insert(std::make_pair("NumpadPeriod", int(Keys::NumpadPeriod)));
        StringToKeycode.insert(std::make_pair("A", int(Keys::A)));
        StringToKeycode.insert(std::make_pair("B", int(Keys::B)));
        StringToKeycode.insert(std::make_pair("C", int(Keys::C)));
        StringToKeycode.insert(std::make_pair("D", int(Keys::D)));
        StringToKeycode.insert(std::make_pair("E", int(Keys::E)));
        StringToKeycode.insert(std::make_pair("F", int(Keys::F)));
        StringToKeycode.insert(std::make_pair("G", int(Keys::G)));
        StringToKeycode.insert(std::make_pair("H", int(Keys::H)));
        StringToKeycode.insert(std::make_pair("I", int(Keys::I)));
        StringToKeycode.insert(std::make_pair("J", int(Keys::J)));
        StringToKeycode.insert(std::make_pair("K", int(Keys::K)));
        StringToKeycode.insert(std::make_pair("L", int(Keys::L)));
        StringToKeycode.insert(std::make_pair("M", int(Keys::M)));
        StringToKeycode.insert(std::make_pair("N", int(Keys::N)));
        StringToKeycode.insert(std::make_pair("O", int(Keys::O)));
        StringToKeycode.insert(std::make_pair("P", int(Keys::P)));
        StringToKeycode.insert(std::make_pair("Q", int(Keys::Q)));
        StringToKeycode.insert(std::make_pair("R", int(Keys::R)));
        StringToKeycode.insert(std::make_pair("S", int(Keys::S)));
        StringToKeycode.insert(std::make_pair("T", int(Keys::T)));
        StringToKeycode.insert(std::make_pair("U", int(Keys::U)));
        StringToKeycode.insert(std::make_pair("V", int(Keys::V)));
        StringToKeycode.insert(std::make_pair("W", int(Keys::W)));
        StringToKeycode.insert(std::make_pair("X", int(Keys::X)));
        StringToKeycode.insert(std::make_pair("Y", int(Keys::Y)));
        StringToKeycode.insert(std::make_pair("Z", int(Keys::Z)));
        StringToKeycode.insert(std::make_pair("F1", int(Keys::F1)));
        StringToKeycode.insert(std::make_pair("F2", int(Keys::F2)));
        StringToKeycode.insert(std::make_pair("F3", int(Keys::F3)));
        StringToKeycode.insert(std::make_pair("F4", int(Keys::F4)));
        StringToKeycode.insert(std::make_pair("F5", int(Keys::F5)));
        StringToKeycode.insert(std::make_pair("F6", int(Keys::F6)));
        StringToKeycode.insert(std::make_pair("F7", int(Keys::F7)));
        StringToKeycode.insert(std::make_pair("F8", int(Keys::F8)));
        StringToKeycode.insert(std::make_pair("F9", int(Keys::F9)));
        StringToKeycode.insert(std::make_pair("F10", int(Keys::F10)));
        StringToKeycode.insert(std::make_pair("F11", int(Keys::F11)));
        StringToKeycode.insert(std::make_pair("F12", int(Keys::F12)));
        StringToKeycode.insert(std::make_pair("Up", int(Keys::Up)));
        StringToKeycode.insert(std::make_pair("Right", int(Keys::Right)));
        StringToKeycode.insert(std::make_pair("Down", int(Keys::Down)));
        StringToKeycode.insert(std::make_pair("Left", int(Keys::Left)));
        StringToKeycode.insert(std::make_pair("Tab", int(Keys::Tab)));
        StringToKeycode.insert(std::make_pair("CapsLock", int(Keys::CapsLock)));
        StringToKeycode.insert(std::make_pair("NumLock", int(Keys::NumLock)));
        StringToKeycode.insert(std::make_pair("ScrollLock", int(Keys::ScrollLock)));
        StringToKeycode.insert(std::make_pair("LeftShift", int(Keys::LeftShift)));
        StringToKeycode.insert(std::make_pair("RightShift", int(Keys::RightShift)));
        StringToKeycode.insert(std::make_pair("LeftControl", int(Keys::LeftControl)));
        StringToKeycode.insert(std::make_pair("RightControl", int(Keys::RightControl)));
        StringToKeycode.insert(std::make_pair("LeftAlt", int(Keys::LeftAlt)));
        StringToKeycode.insert(std::make_pair("RightAlt", int(Keys::RightAlt)));
        StringToKeycode.insert(std::make_pair("Backspace", int(Keys::Backspace)));
        StringToKeycode.insert(std::make_pair("Return", int(Keys::Return)));
        StringToKeycode.insert(std::make_pair("NumpadEnter", int(Keys::NumpadEnter)));
        StringToKeycode.insert(std::make_pair("Insert", int(Keys::Insert)));
        StringToKeycode.insert(std::make_pair("Delete", int(Keys::Delete)));
        StringToKeycode.insert(std::make_pair("PageUp", int(Keys::PageUp)));
        StringToKeycode.insert(std::make_pair("PageDown", int(Keys::PageDown)));
        StringToKeycode.insert(std::make_pair("Escape", int(Keys::Escape)));
        StringToKeycode.insert(std::make_pair("Space", int(Keys::Space)));
        StringToKeycode.insert(std::make_pair("BackQuote", int(Keys::BackQuote)));
        StringToKeycode.insert(std::make_pair("Minus", int(Keys::Minus)));
        StringToKeycode.insert(std::make_pair("Equals", int(Keys::Equals)));
        StringToKeycode.insert(std::make_pair("LeftBracket", int(Keys::LeftBracket)));
        StringToKeycode.insert(std::make_pair("RightBracket", int(Keys::RightBracket)));
        StringToKeycode.insert(std::make_pair("Semicolon", int(Keys::Semicolon)));
        StringToKeycode.insert(std::make_pair("Quote", int(Keys::Quote)));
        StringToKeycode.insert(std::make_pair("Period", int(Keys::Period)));
        StringToKeycode.insert(std::make_pair("BackSlash", int(Keys::BackSlash)));
        StringToKeycode.insert(std::make_pair("Slash", int(Keys::Slash)));

//        for(std::unordered_map<std::string, int>::iterator i = StringToKeycode.begin(); i != StringToKeycode.end(); ++i) {
//            std::cout << (*i).first << ":" << (*i).second << std::endl;
//        }
    }

    std::vector<std::string> Keys::GetKeyStrings() {
        std::vector<std::string> keyStrings;
        keyStrings.reserve(StringToKeycode.size());
        for(std::unordered_map<std::string, int>::iterator i = StringToKeycode.begin(); i != StringToKeycode.end(); ++i) {
            keyStrings.push_back((*i).first);
        }
        return keyStrings;
    }

    int Keys::GetKeyFromString(std::string const& key) {
        std::unordered_map<std::string, int>::iterator result = StringToKeycode.find(key);
        if(result != StringToKeycode.end()) {
            return (*result).second;
        }
        else {
            return 0;
        }
    }
}
