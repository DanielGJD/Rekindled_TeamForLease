#include "ComponentEnemyPace.h"
#include "ComponentPhysics.h"
#include "ComponentTransform.h"
#include "SoundEmitter.h"
#include "Entity.h"
#include "Engine.h"
#include "Vector.h"

namespace ForLeaseEngine
{
    namespace Components
    {
            EnemyPace::EnemyPace(Entity& owner, float speed, float maxDistance, float pause): Component(owner),
                                                                                              PaceSound(""),
                                                                                              PaceSpeed(speed),
                                                                                              MaxPaceDistance(maxDistance),
                                                                                              PauseTimer(pause),
                                                                                              Position(Parent.GetComponent<Components::Transform>()->Position)
            {
                CurrentAction = Action::LEFT;
                NextAction = Action::PAUSE;
                Direction = 0;
                Moved = 0;
                timer = PauseTimer;
            }

            EnemyPace * EnemyPace::Create(Entity& owner)
            {
                EnemyPace * pace = new EnemyPace(owner);
                pace->Initialize();
                return pace;
            }

            EnemyPace::~EnemyPace()
            {
                //ForLease->Dispatcher.Detach(this, "EntitiesSeen");
            }

            void EnemyPace::Initialize()
            {
                //ForLease->Dispatcher.Attach(NULL, this, "EntitiesSeen", &EnemyPace::OnPlayerSeen);
            }

            void EnemyPace::Update()
            {
                float dt = ForLease->FrameRateController().GetDt();
                switch (CurrentAction)
                {
                case Action::PAUSE:
                    MovePause(dt);
                    break;
                case Action::LEFT:
                    MoveLeft(dt);
                    break;
                case Action::RIGHT:
                    MoveRight(dt);
                    break;
                }
//                Components::Transform* trans = Parent.GetComponent<Components::Transform>();
//                Point EnemyPos = Parent.GetComponent<Components::Transform>()->Position;
//
//                //Components::Physics * EnemyBody = Parent.GetComponent<Component::Phyiscs>();
//
//
//               // Components::Transform * entity2Transform = entity2->GetComponent<Components::Transform>();
//                //Point entity1Position = entity1Transform->Position;
//
//                //bool Detected = true;
//                //bool MoveRight = true;
//                //bool MoveLeft = false;
//                //float Pacedistance = 0.1;
//
//                Physics * EnemyBody = Parent.GetComponent<Physics>();
//                if(Detected && MoveRight && !MoveLeft)
//                {
//                    //Entity * Enemy = ForLease->GameStateManager()->CurrentState()->GetEntityByName("Enemy");
//                    //Point EnemyPosition = Enemy->GetComponent<Components::Transform>()->Position;
//                    //EnemyPosition += Pacedistance * ForLease->FrameRateController().GetDt();
//
//                    EnemyBody->Velocity[0] = 2.0;
//                    float MaxPaceDistance = 5.0;
//                    if(EnemyPos[0] > EnemyPos[0] + MaxPaceDistance)
//                    {
//                        MoveRight = false;
//                        MoveLeft = true;
//                        if(!MoveRight)
//                        {
//                            EnemyBody->Velocity[0] = 0;
//                            //ForLease->FrameRateController().GetDt();
//                        }
//
//                    }
//
//                }
//                else if (Detected && MoveLeft && !MoveRight)
//                {
//                    EnemyBody->Velocity[0] = -2.0;
//                    if(EnemyPos[0] < (EnemyPos[0] + -MaxPaceDistance))
//                    {
//                        MoveLeft = false;
//                        MoveRight = true;
//                        if(!MoveLeft)
//                        {
//                            EnemyBody->Velocity[0] = 0;
//                        }
//                    }
//                }
            }


            void EnemyPace::MoveLeft(float dt)
            {
                Position[Direction] -= PaceSpeed * dt;
                Moved += PaceSpeed * dt;

                if (Moved >= MaxPaceDistance)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::RIGHT;
                    Moved = -MaxPaceDistance;
                }
            }

            void EnemyPace::MoveRight(float dt)
            {
                Position[Direction] += PaceSpeed * dt;
                Moved += PaceSpeed * dt;

                if (Moved >= MaxPaceDistance)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::LEFT;
                    Moved = -MaxPaceDistance;
                }
            }

            void EnemyPace::MovePause(float dt)
            {
                timer -= dt;
                if (timer <= 0)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::PAUSE;
                    timer = PauseTimer;
                    Components::Model* model = Parent.GetComponent<Components::Model>();
                    if (model)
                        model->FlipY = !(model->FlipY);
                }
            }
            void EnemyPace::Serialize(Serializer& root)
            {
                root.WriteUint("Type", static_cast<unsigned long long>(Type));
                Serializer pace = root.GetChild("EnemyPace");
                pace.WriteFloat("MaxPaceDistance", MaxPaceDistance);
                pace.WriteFloat("PaceSpeed", PaceSpeed);
                pace.WriteFloat("PauseTimer", PauseTimer);
                pace.WriteInt("Direction", Direction);
                pace.WriteUint("Type", static_cast<unsigned long long>(Type));
                root.Append(pace, "EnemyPace");
            }

            void EnemyPace::Deserialize(Serializer& root)
            {
                Serializer pace = root.GetChild("EnemyPace");
                pace.ReadFloat("MaxPaceDistance", MaxPaceDistance);
                pace.ReadFloat("MaxPaceDistance", MaxPaceDistance);
                pace.ReadFloat("PaceSpeed", PaceSpeed);
                pace.ReadFloat("PauseTimer", PauseTimer);
                pace.ReadInt("Direction", Direction);
            }



    }
}
