/*!
    \file   MenuItems.h
    \author Sean McGeer
    \date   11/20/15

    \brief
        Defines menu items.

    \see ComponentMenu.h
    \see MenuItems.cpp

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MENU_ITEMS_H
#define MENU_ITEMS_H

#include "Serializable.h"
#include <string>

namespace ForLeaseEngine {

    enum class MenuItemType : unsigned {
        None                    = 0,
        LoadLevel               = 1,
        NextLevel               = 2,
        Quit                    = 3,
        ActivateOther           = 4,
        ActivateAndDeactivate   = 5,
        ResumeGame              = 6,
        ActDeactVisible         = 7,
        ActDeactInvisible       = 8,
        OptionResolution        = 9,
        OptionVolume            = 10,
        OptionAccept            = 11,
        OptionFullscreen        = 12,
        OptionBorderless        = 13
    };

    class MenuItem : public Serializable {
        public:
            MenuItem() {}
            MenuItem(MenuItemType type, std::string text, bool option = false);
            std::string Text;
            MenuItemType Type;
            bool Option;
            virtual void Action() { }
            virtual void Serialize(Serializer& root);
            virtual void Deserialize(Serializer& root);
    };

    namespace MenuItems {

        class LoadLevel : public MenuItem {
            public:
                LoadLevel(std::string image, std::string stateName);
                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string StateName;
        };

        class NextLevel : public MenuItem {
            public:
                NextLevel(std::string image);
                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);
        };

        class Quit : public MenuItem {
            public:
                Quit(std::string image);
                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);
        };

        class ActivateOther : public MenuItem {
            public:
                ActivateOther(std::string image, std::string otherMenu);

                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string OtherMenu;
        };

        class ActivateAndDeactivate : public MenuItem {
            public:
                ActivateAndDeactivate(std::string image, std::string toActivate, std::string toDeactivate);

                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string ToDeactivate;
                std::string ToActivate;
        };

        class ResumeGame : public MenuItem {
            public:
                ResumeGame(std::string image);

                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);
        };

        class ActivateAndDeactivateAndMakeInvisible : public MenuItem {
            public:
                ActivateAndDeactivateAndMakeInvisible(std::string image, std::string toActivate, std::string toDeactivate, std::string toMakeInvisible);
                
                virtual void Action();
                
                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string ToDeactivate;
                std::string ToActivate;
                std::string ToMakeInvisible;
        };

        class ActivateAndDeactivateAndMakeVisible : public MenuItem {
            public:
                ActivateAndDeactivateAndMakeVisible(std::string image, std::string toActivate, std::string toDeactivate, std::string toMakeVisible);

                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string ToDeactivate;
                std::string ToActivate;
                std::string ToMakeVisible;
        };

    } // MenuItems

    class OptionMenuItem : public MenuItem {
        public:
            OptionMenuItem(MenuItemType type, std::string text);
            virtual void Accept() = 0;
            bool Dirty = false;
            std::string FirstText;
    };

    namespace OptionMenuItems {
        class FinalAccept : public OptionMenuItem {
            public:
                FinalAccept(std::string parentMenuName, std::string text = "Accept");
                void Action();
                void Accept();
                std::string ParentMenuName;
        };

        class Resolution : public OptionMenuItem {
            class ItemizedResolution {
                public:
                    struct Res {
                        Res(int x, int y) : X(x), Y(y) {}
                        int X;
                        int Y;
                    };

                    ItemizedResolution();

                    void SetIndex(int x, int y);
                    Res GetResolution();
                    void IncrementIndex();
                    Res GetNextResolution();
                private:
                    std::vector<Res> Resolutions;
                    unsigned Index;
            };

            ItemizedResolution Current;
            public:
                Resolution(std::string text = "Resolution: ");
                void Action();
                void SetText();
                void Accept();
        };

        class Fullscreen : public OptionMenuItem {
            public:
                Fullscreen(std::string text = "Fullscreen: ");
                void SetText();
                void Action();
                void Accept();
                bool IsFullscreen;
        };

        class Volume : public OptionMenuItem {
            public:
                Volume(std::string text = "Volume: ", unsigned volumeIncrement = 10, unsigned maxVolume = 100);
                void SetText();
                void Action();
                void Accept();
                unsigned CurrentVolume;
                unsigned VolumeIncrement;
                unsigned MaxVolume;
        };

    } // OptionMenuItems

} // ForLeaseEngine

#endif // MENU_ITEMS_H