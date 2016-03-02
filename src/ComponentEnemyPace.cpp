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
        EnemyPace::EnemyPace(Entity& owner):Component(owner, ComponentType::Physics | ComponentType::Collision),
                                            Detected(true), MoveRight(true), MoveLeft(false), PaceSpeed(2.0),PaceDistance(0.1),
                                            MaxPaceDistance(5.0), PaceSound("")
            {

               // Entity * enemy = ForLease->GameStateManager().CurrentState().GetEntityByName("Enemy");



            }

            EnemyPace * EnemyPace::Create(Entity& owner)
            {
                EnemyPace * pace = new EnemyPace(owner);
                pace->Initialize();
                return pace;
            }

            EnemyPace::~EnemyPace()
            {
                ForLease->Dispatcher.Detach(this, "EntitiesScreen");
            }

            void EnemyPace::Initialize()
            {

            }

            void EnemyPace::Update()
            {
                Components::Transform* trans = Parent.GetComponent<Components::Transform>();
                Point EnemyPos = Parent.GetComponent<Components::Transform>()->Position;

                //Components::Physics * EnemyBody = Parent.GetComponent<Component::Phyiscs>();


               // Components::Transform * entity2Transform = entity2->GetComponent<Components::Transform>();
                //Point entity1Position = entity1Transform->Position;

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
                    float MaxPaceDistance = 5.0;
                    if(EnemyPos.GetX() > EnemyPos.GetX() + MaxPaceDistance)
                    {
                        MoveRight = false;
                        MoveLeft = true;
                        if(!MoveRight)
                        {
                            EnemyBody->Velocity[0] = 0;
                            //ForLease->FrameRateController().GetDt();
                        }

                    }

                }
                else if (Detected && MoveLeft && !MoveRight)
                {
                    EnemyBody->Velocity[0] = -2.0;
                    if(EnemyPos.x < (EnemyPos.x + -MaxPaceDistance))
                    {
                        MoveLeft = false;
                        MoveRight = true;
                        if(!MoveLeft)
                        {
                            EnemyBody->Velocity[0] = 0;
                        }
                    }
                }
            }


            void EnemyPace::Serialize(Serializer& root)
            {
                root.WriteUint("Type", static_cast<unsigned>(Type));
                Serializer controller = root.GetChild("EnemyPace");
                controller.WriteFloat("MaxPaceDistance", MaxPaceDistance);
                controller.WriteUint("Type", static_cast<unsigned>(Type));
                root.Append(pace, "EnemyPace");
            }

            void EnemyPace::Deserialize(Serializer& root)
            {
                root.WriteUint("Type", static_cast<unsigned>(Type));
                Serializer controller = root.GetChild("EnemyPace");
                controller.WriteFloat("MaxPaceDistance", MaxPaceDistance);
                controller.WriteUint("Type", static_cast<unsigned>(Type));
                root.Append(pace, "EnemyPace");
            }



    }
}
