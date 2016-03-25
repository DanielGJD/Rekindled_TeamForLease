/*!
    \file   ComponentEnemyAI.h
    \author Christopher Hudson

    \brief
        Simple, non moving ai that detects the player and causes loss if player seen too long

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentEnemyAI.h"
#include "ComponentVisionCone.h"
#include "Entity.h"
#include "MultiEntityEvent.h"
#include "State.h"
#include "GameStateManager.h"
#include "ComponentSoundEmitter.h"
#include "DamageEvent.h"
#include <iostream>

namespace ForLeaseEngine {
    namespace Components {
        EnemyAI::EnemyAI(Entity& parent,
                         bool active,
                         Color const& noDetectionColor,
                         Color const& detectionColor,
                         Color const& happyColor,
                         float detectionDelay,
                         std::string const& hatedEntityName,
                         std::string const& likedEntityName,
                         std::string const& hatedSeenSound,
                         std::string const& likedSeenSound)
                         : Component(parent),
                           Active(active), NoDetectionColor(noDetectionColor),
                           DetectionColor(detectionColor), HappyColor(happyColor),
                           DetectionDelay(detectionDelay), HatedEntityName(hatedEntityName),
                           LikedEntityName(likedEntityName), HatedSeenSound(hatedSeenSound),
                           LikedSeenSound(likedSeenSound), Happy(false), Angry(false),
                           DetectionTimer(0) {}

        EnemyAI::~EnemyAI() {
            //Detach from dispatcher
            ForLease->Dispatcher.Detach(this, "EntitiesSeen");
        }

        void EnemyAI::Initialize() {
            //Attach to dispatcher
            ForLease->Dispatcher.Attach(NULL, this, "EntitiesSeen", &EnemyAI::OnEntitiesSeen, &Parent);
        }

        EnemyAI* EnemyAI::Create(Entity& parent) {
            EnemyAI* enemyAI = new EnemyAI(parent);
            enemyAI->Initialize();
            return enemyAI;
        }

        ComponentType EnemyAI::GetType() { return Type; }

        void EnemyAI::Update() {
            //Update timer and colors
//            if(!Active) {
//                return;
//            }
//
//            Components::VisionCone* cone = Parent.GetComponent<Components::VisionCone>();
//
//            if(Happy) {
//                DetectionTimer = 0;
//                cone->IndicatorColor = HappyColor;
//            }
//            else {
//                if(Angry) {
//                    //cone->IndicatorColor = DetectionColor;
//                    DetectionTimer += ForLease->FrameRateController().GetDt();
//                    if(DetectionTimer > DetectionDelay) {
//                        DetectionTimer = DetectionDelay;
//                        // Also, you lose!!!!!
//                        ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
//                    }
//                    // Option 1
//                    /*float timeSwitch = DetectionDelay / 2;
//
//                    if(DetectionTimer < timeSwitch) {
//                        float t = DetectionTimer / timeSwitch;
//                        cone->IndicatorColor = Color(NoDetectionColor.GetR() + DetectionColor.GetR() * t,
//                                                     NoDetectionColor.GetG() + DetectionColor.GetG() * t,
//                                                     NoDetectionColor.GetB() + DetectionColor.GetB() * t,
//                                                     NoDetectionColor.GetA() + DetectionColor.GetA() * t);
//                    }
//                    else {
//                        float t = 1 - (DetectionTimer - timeSwitch) / timeSwitch;
//                        cone->IndicatorColor = Color(NoDetectionColor.GetR() * t + DetectionColor.GetR(),
//                                                     NoDetectionColor.GetG() * t + DetectionColor.GetG(),
//                                                     NoDetectionColor.GetB() * t + DetectionColor.GetB(),
//                                                     NoDetectionColor.GetA() * t + DetectionColor.GetA());
//                    }*/
//
//                    // Option 2
//                    float badPart = DetectionTimer / DetectionDelay;
//                    float goodPart = 1 - badPart;
//
//                    cone->IndicatorColor = Color(NoDetectionColor.GetR() * goodPart + DetectionColor.GetR() * badPart,
//                                                 NoDetectionColor.GetG() * goodPart + DetectionColor.GetG() * badPart,
//                                                 NoDetectionColor.GetB() * goodPart + DetectionColor.GetB() * badPart,
//                                                 NoDetectionColor.GetA() * goodPart + DetectionColor.GetA() * badPart);
//                }
//                else {
//                    cone->IndicatorColor = NoDetectionColor;
//                    DetectionTimer = 0;
//                }
//            }
        }

        void EnemyAI::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer enemyAI = root.GetChild("EnemyAI");
            enemyAI.WriteUint("Type", static_cast<unsigned int>(Type));
            enemyAI.WriteBool("Active", Active);
            enemyAI.WriteFloat("DetectionDelay", DetectionDelay);
            enemyAI.WriteString("HatedEntityName", HatedEntityName);
            enemyAI.WriteString("LikedEntityName", LikedEntityName);
            enemyAI.WriteString("HatedSeenSound", HatedSeenSound);
            enemyAI.WriteString("LikedSeenSound", LikedSeenSound);
            Serializer noDetect = enemyAI.GetChild("NoDetectionColor");
            NoDetectionColor.Serialize(noDetect);
            enemyAI.Append(noDetect, "NoDetectionColor");
            Serializer detect = enemyAI.GetChild("DetectionColor");
            DetectionColor.Serialize(detect);
            enemyAI.Append(detect, "DetectionColor");
            Serializer happy = enemyAI.GetChild("HappyColor");
            HappyColor.Serialize(happy);
            enemyAI.Append(happy, "HappyColor");
            root.Append(enemyAI, "EnemyAI");
        }

        void EnemyAI::Deserialize(Serializer& root) {
            Serializer enemyAI = root.GetChild("EnemyAI");
            enemyAI.ReadBool("Active", Active);
            enemyAI.ReadFloat("DetectionDelay", DetectionDelay);
            enemyAI.ReadString("HatedEntityName", HatedEntityName);
            enemyAI.ReadString("LikedEntityName", LikedEntityName);
            enemyAI.ReadString("HatedSeenSound", HatedSeenSound);
            enemyAI.ReadString("LikedSeenSound", LikedSeenSound);
            Serializer noDetect = enemyAI.GetChild("NoDetectionColor");
            NoDetectionColor.Deserialize(noDetect);
            Serializer detect = enemyAI.GetChild("DetectionColor");
            DetectionColor.Deserialize(detect);
            Serializer happy = enemyAI.GetChild("HappyColor");
            HappyColor.Deserialize(happy);
        }

        void EnemyAI::OnEntitiesSeen(Event const* e) {
            MultiEntityEvent const* multi_e = static_cast<MultiEntityEvent const*>(e);
            bool wasHappy = Happy;
            bool wasAngry = Angry;
            Happy = false;
            Angry = false;
            bool willBeAngry = false;
            Entity* hatedEntity = NULL;
            for(unsigned int i = 0; i < multi_e->EntityIDs.size(); ++i) {
                Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityByID(multi_e->EntityIDs[i]);
                LevelComponents::Light* lightSystem = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Light>();
                std::string entityName = entity->GetName();

                if(entityName.compare(LikedEntityName) == 0) {
                    // not using lighting
                    if(!lightSystem) {
                        Happy = true;
                        break; // Don't need to continue if happy since it overwrites angry
                    }
                    // using lighting
                    else {
                        if(lightSystem->CheckIfLit(multi_e->EntityIDs[i])) {
                            Happy = true;
                            break;
                        }
                    }
                }
                else if(entityName.compare(HatedEntityName) == 0) {
                    //std::cout << "Hated entity in line of sight" << std::endl;
                    if(!lightSystem) {
                        willBeAngry = true;
                        hatedEntity = entity;
                    }
                    else {
                        if(lightSystem->CheckIfLit(multi_e->EntityIDs[i])) {
                            //std::cout << "Hated entity lit" << std::endl;
                            willBeAngry = true;
                            hatedEntity = entity;
                        }
                    }
                }
            }
            if(!Happy) {
                Angry = willBeAngry;
            }

            Components::SoundEmitter* emitter = Parent.GetComponent<Components::SoundEmitter>();

            if(!wasHappy && Happy) {
                if(emitter) {
                    emitter->SetVolume(1.0f, LikedSeenSound);
                    emitter->StopEvent(LikedSeenSound);
                    emitter->PlayEvent(LikedSeenSound);
                }
                //std::cout << "Object Seen" << std::endl;
                Event e = Event("ObjectSeen");
                ForLease->Dispatcher.DispatchToParent(&e, &Parent);
            }
            else if(wasHappy && !Happy) {
                //std::cout << "Object not Seen" << std::endl;
                Event e = Event("ObjectNotSeen");
                ForLease->Dispatcher.DispatchToParent(&e, &Parent);
            }
            else if(!wasAngry && Angry) {
                if(emitter) {
                    emitter->SetVolume(1.0f,HatedSeenSound);
                    emitter->StopEvent(HatedSeenSound);
                    emitter->PlayEvent(HatedSeenSound);
                }
                //std::cout << "Object Seen" << std::endl;
                Event e = Event("ObjectSeen");
                ForLease->Dispatcher.DispatchToParent(&e, &Parent);
            }
            else if(wasAngry && !Angry) {
                //std::cout << "Object not Seen" << std::endl;
                Event e = Event("ObjectNotSeen");
                ForLease->Dispatcher.DispatchToParent(&e, &Parent);
            }


            if(!Happy && hatedEntity) {
                //std::cout << "Sending " << damage.Damage << " to " << hatedEntity->GetName() << " : " << hatedEntity << std::endl;
                DamageEvent damage = DamageEvent(ForLease->FrameRateController().GetDt());
                ForLease->Dispatcher.DispatchToParent(&damage, hatedEntity);
            }
        }
    }
}
