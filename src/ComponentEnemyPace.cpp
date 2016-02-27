#include "ComponentEnemyPace.h"
#include "ComponentPhysics.h"
#include "SoundEmitter.h"
#include "Entity.h"
#include "Engine.h"

namespace ForLeaseEngine
{
    namespace Components
    {
        EnemyPace::EnemyPace(Entity& owner): Component(owner, ComponentType::Physics | ComponentType::Collision),
                                            Detected(true), MoveRight(true), MoveLeft(false), PaceSpeed(2.0),PaceDistance(0.1),
                                            MaxPaceDistance(5.0), PaceSound(""){};

            EnemyPace * EnemyPace::Create(Entity* owner)
            {
                EnemyPace* pace = new EnemyPace(owner);
                pace->Initialize();
                return pace;
            }

            EnemyPace::~EnemyPace
            {

            }

            void EnemyPace::Initialize()
            {

            }

            void EnemyPace::Update()
            {
                Components::Model * EnemyModel = Parent.GetComponent<Component::Model>();
                //Components::Physics * EnemyBody = Parent.GetComponent<Component::Phyiscs>();
                Components::Transform * EnemyTrans = Parent.GetComponent<Component::Transform>();

                //bool Detected = true;
                //bool MoveRight = true;
                //bool MoveLeft = false;
                //float Pacedistance = 0.1;
                Physics * EnemyBody = Parent.GetComponent<Physics>();
                if(Detected && MoveRight && !MoveLeft)
                {
                    //Entity * Enemy = ForLease->GameStateManager()->CurrentState()->GetEntityByName("Enemy");
                    //Point EnemyPosition = Enemy->GetComponent<Components::Transform>()->Position;
                    //EnemyPosition += Pacedistance * ForLease->FrameRateController().GetDt();
                    EnemyBody->Velocity[0] = 2.0;
                    if(EnemyBody->Velocity[0] > maxPaceDistance)
                    {
                        MoveRight = false;

                        //printf("updateV %f\n", rbody->Velocity[0]);
                        rbody->Velocity[0] = 0;


                    }



                }
                else if (Detected && MoveLeft && !MoveRight)
                {
                    EnemyBody->Velocity[0] = -2.0;
                    if(EnemyBody->Velocity[0] < -MaxPaceDistance)
                    {
                        //printf("negativeV %f\n", rbody->Velocity[0]);
//                        rbody->Velocity[0] = rbody->Velocity[0] + PaceDistance;
//                        if(rbody->Velocity[0] > -MaxPaceDistance)
//                            rbody->Velocity[0] = rbody->Velocity[0] - PaceDistance;
                        MoveRight = true;
                        EnemyBody->Velocity[0] = 0;

                    }
                }
            }


            void EnemyPace::Serialize(Serializer& root)
            {
                root.WriteUint("Type", static_cast<unsigned>(Type));
                Serializer controller = root.GetChild("EnemyPace");
                controller.WriteFloat("MoveSpeed", MoveSpeed);
                controller.WriteFloat("JumpSpeed", JumpSpeed);
                controller.WriteFloat("Drag", Drag);
                controller.WriteFloat("maxSpeed", maxSpeed);
                controller.WriteFloat("PaceDistance", PaceDistance);
                controller.WriteString("EnemyWalkAnimation", WalkAnimation);
                controller.WriteUint("Type", static_cast<unsigned>(Type));
                root.Append(pace, "EnemyPace");
            }

            void EnemyPace::Deserialize(Serializer& root)
            {
                root.WriteUint("Type", static_cast<unsigned>(Type));
                Serializer controller = root.GetChild("EnemyPace");
                controller.WriteFloat("MoveSpeed", MoveSpeed);
                controller.WriteFloat("JumpSpeed", JumpSpeed);
                controller.WriteFloat("Drag", Drag);
                controller.WriteFloat("maxSpeed", maxSpeed);
                controller.WriteFloat("PaceDistance", PaceDistance);
                controller.WriteString("EnemyWalkAnimation", WalkAnimation);
                controller.WriteUint("Type", static_cast<unsigned>(Type));
                root.Append(pace, "EnemyPace");
            }



    }
}
