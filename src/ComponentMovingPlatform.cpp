/*!
    \file   ComponentMovingPlatform.cpp
    \author Sean McGeer
    \author Sam Montanari
    \date   3/23/16
    \brief
        Implements the Moving Platform component.
    \see ComponentMovingPlatform.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentEnemyPace.h"
#include "ComponentPhysics.h"
#include "ComponentTransform.h"
#include "SoundEmitter.h"
#include "Entity.h"
#include "Engine.h"
#include "Vector.h"
#include "State.h"

namespace ForLeaseEngine
{
    namespace Components
    {
            MovingPlatform::MovingPlatform(Entity& owner, float awaySpeed, float backSpeed, float affectedFieldHeight, float maxDistance, float pause, Axis direction) : Component(owner, ComponentType::Transform | ComponentType::Collision | ComponentType::Physics),
                                                                                                                                              AwaySpeed(awaySpeed),
                                                                                                                                              BackSpeed(backSpeed),
                                                                                                                                              AffectedFieldHeight(affectedFieldHeight),
                                                                                                                                              MaxMove(maxDistance),
                                                                                                                                              PauseTimer(pause),
                                                                                                                                              Direction(direction)
            {
                CurrentAction = Action::Away;
                NextAction = Action::Away;
                Moved = 0;
                CurrentPauseTimer = 0;
            }

            MovingPlatform::~MovingPlatform()
            {
            }

            void MovingPlatform::Update()
            {
                float dt = ForLease->FrameRateController().GetDt();

                switch (CurrentAction)
                {
                case Action::Pause:
                    MovePause(dt);
                    break;
                case Action::Away:
                    MoveAway(dt);
                    break;
                case Action::Back:
                    MoveBack(dt);
                    break;
                }
            }


            void MovingPlatform::MoveAway(float dt)
            {
                Components::Physics* physics = Parent.GetComponent<Components::Physics>();

                if (Direction == Axis::Horizontal)
                    physics->Velocity[0] = -AwaySpeed;
                else
                    physics->Velocity[1] = -AwaySpeed;

                Moved += AwaySpeed * dt;

                if (Moved >= MaxMove)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::Back;
                    Moved = -MaxMove;
                }
            }

            void MovingPlatform::MoveBack(float dt)
            {
                Components::Physics* physics = Parent.GetComponent<Components::Physics>();

                if (Direction == Axis::Horizontal)
                    physics->Velocity[0] = BackSpeed;
                else
                    physics->Velocity[1] = BackSpeed;

                Moved += BackSpeed * dt;

                if (Moved >= MaxMove)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::Away;
                    Moved = -MaxMove;
                }
            }

            void MovingPlatform::MovePause(float dt)
            {
                CurrentPauseTimer += dt;
                Components::Physics* physics = Parent.GetComponent<Components::Physics>();
                physics->Velocity = Vector(0, 0);

                if (CurrentPauseTimer >= PauseTimer)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::Pause;
                    CurrentPauseTimer = 0;
                    Components::Model* model = Parent.GetComponent<Components::Model>();
                    if (model)
                        model->FlipY = !(model->FlipY);
                }
            }

            void MovingPlatform::Serialize(Serializer& root)
            {
                root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
                Serializer plat = root.GetChild("MovingPlatform");
                plat.WriteFloat("MaxMove", MaxMove);
                plat.WriteFloat("AwaySpeed", AwaySpeed);
                plat.WriteFloat("BackSpeed", BackSpeed);
                plat.WriteFloat("AffectedFieldHeight", AffectedFieldHeight);
                plat.WriteFloat("PauseTimer", PauseTimer);
                plat.WriteInt("Direction", static_cast<int>(Direction));
                plat.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
                root.Append(plat, "MovingPlatform");
            }

            void MovingPlatform::Deserialize(Serializer& root)
            {
                Serializer plat = root.GetChild("MovingPlatform");
                plat.ReadFloat("MaxMove", MaxMove);
                plat.ReadFloat("AwaySpeed", AwaySpeed);
                plat.ReadFloat("BackSpeed", BackSpeed);
                plat.ReadFloat("AffectedFieldHeight", AffectedFieldHeight);
                plat.ReadFloat("PauseTimer", PauseTimer);
                int direction;
                plat.ReadInt("Direction", direction);
                Direction = static_cast<Axis>(direction);
            }

            Vector MovingPlatform::LastMovement(bool scaleDt) {
                Vector movement(0,0);
                unsigned direction;

                if (Direction == Axis::Horizontal) direction = 0;
                else direction = 1;

                float dt;
                if (scaleDt)
                    dt = ForLease->FrameRateController().GetDt();
                else
                    dt = 1.0f;

                if (CurrentAction == Action::Away)
                    movement[direction] = -(AwaySpeed * dt);
                else if (CurrentAction == Action::Back)
                    movement[direction] = (BackSpeed * dt);

                return movement;
            }
    }
}
