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
            MovingPlatform::MovingPlatform(Entity& owner, float speed, float maxDistance, float pause, unsigned direction): Component(owner, ComponentType::Transform | ComponentType::Collision | ComponentType::Physics),
                                                                                              PaceSound(""),
                                                                                              PaceSpeed(speed),
                                                                                              MaxPaceDistance(maxDistance),
                                                                                              PauseTimer(pause),
                                                                                              //Position(Parent.GetComponent<Components::Transform>()->Position),
                                                                                              Direction(direction)
            {
                MaxPaceDistance = 3.0;
                DetectionDelay = 0.5;
                CurrentAction = Action::Left;
                NextAction = Action::Pause;
                //Direction = 0;
                Moved = 0;
                CurrentPauseTimer = 0;
            }

            MovingPlatform * MovingPlatform::Create(Entity& owner)
            {
                MovingPlatform * pace = new MovingPlatform(owner);
                pace->Initialize();
                return pace;
            }

            MovingPlatform::~MovingPlatform()
            {
                ForLease->Dispatcher.Detach(this, "PlayerSeen");
                ForLease->Dispatcher.Detach(this, "PlayerNotSeen");
            }

            void MovingPlatform::Initialize()
            {
                ForLease->Dispatcher.Attach(NULL, this, "PlayerSeen", &EnemyPace::OnPlayerSeen, &Parent);
                ForLease->Dispatcher.Attach(NULL, this, "PlayerNotSeen", &EnemyPace::OnPlayerNotSeen, &Parent);
            }

            void MovingPlatform::Update()
            {
                float dt = ForLease->FrameRateController().GetDt();

                switch (CurrentAction)
                {
                case Action::Pause:
                    MovePause(dt);
                    break;
                case Action::Left:
                    MoveLeft(dt);
                    break;
                case Action::Right:
                    MoveRight(dt);
                    break;
                }
            }


            void MovingPlatform::MoveLeft(float dt)
            {
                //Position[Direction] += -PaceSpeed * dt;
                Moved += PaceSpeed * dt;

                //Components::Collision* collision = Parent.GetComponent<Components::Collision>();
                //if (collision) {
                //    std::vector<Entity*> entities = ForLease->GameStateManager().CurrentState().GetEntitiesInBox(Position, collision->ScaledWidth(), collision->ScaledHeight());
                //    for (Entity* entity : entities) {
                //        if (entity->HasComponent(ComponentType::Physics)) {
                //            entity->GetComponent<Components::Physics>()->Velocity[Direction] = 0;
                //            entity->GetComponent<Components::Transform>()->Position += LastMovement();
                //        }
                //    }
                //}

                if (Moved >= MaxPaceDistance)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::Right;
                    Moved = -MaxPaceDistance;
                }
            }

            void MovingPlatform::MoveRight(float dt)
            {
                //Position[Direction] += PaceSpeed * dt;
                Moved += PaceSpeed * dt;

                //Components::Collision* collision = Parent.GetComponent<Components::Collision>();
                //if (collision) {
                //    std::vector<Entity*> entities = ForLease->GameStateManager().CurrentState().GetEntitiesInBox(Position, collision->ScaledWidth(), collision->ScaledHeight());
                //    for (Entity* entity : entities) {
                //        if (entity->HasComponent(ComponentType::Physics)) {
                //            entity->GetComponent<Components::Physics>()->Velocity[Direction] = 0;
                //            entity->GetComponent<Components::Transform>()->Position += LastMovement();
                //        }
                //    }
                //}

                if (Moved >= MaxPaceDistance)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::Left;
                    Moved = -MaxPaceDistance;
                }
            }

            void MovingPlatform::MovePause(float dt)
            {
                CurrentPauseTimer += dt;
                //Position[Direction] = 0;
                if (CurrentPauseTimer >= PauseTimer)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::Pause;
                    CurrentPauseTimer = 0;
                    Components::Model* model = Parent.GetComponent<Components::Model>();
                    Components::VisionCone* vision = Parent.GetComponent<Components::VisionCone>();
                    if (model)
                        model->FlipY = !(model->FlipY);
                    if (vision)
                        vision->Direction[0] *= -1;
                }
            }

            void MovingPlatform::Serialize(Serializer& root)
            {
                root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
                Serializer plat = root.GetChild("MovingPlatform");
                plat.WriteFloat("MaxPaceDistance", MaxPaceDistance);
                plat.WriteFloat("PaceSpeed", PaceSpeed);
                plat.WriteFloat("PauseTimer", PauseTimer);
                plat.WriteInt("Direction", Direction);
                plat.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
                root.Append(plat, "MovingPlatform");
            }

            void MovingPlatform::Deserialize(Serializer& root)
            {
                Serializer plat = root.GetChild("MovingPlatform");
                plat.ReadFloat("MaxPaceDistance", MaxPaceDistance);
                plat.ReadFloat("MaxPaceDistance", MaxPaceDistance);
                plat.ReadFloat("PaceSpeed", PaceSpeed);
                plat.ReadFloat("PauseTimer", PauseTimer);
                plat.ReadInt("Direction", Direction);
            }

            Vector MovingPlatform::LastMovement(bool scaleDt) {
                Vector movement(0,0);

                float dt;
                if (scaleDt)
                    dt = ForLease->FrameRateController().GetDt();
                else
                    dt = 1.0f;

                if (CurrentAction == Action::Left)
                    movement[Direction] = -(PaceSpeed * dt);
                else if (CurrentAction == Action::Right)
                    movement[Direction] = (PaceSpeed * dt);

                return movement;
            }
    }
}
