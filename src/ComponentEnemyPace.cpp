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
            EnemyPace::EnemyPace(Entity& owner, float speed, float maxDistance, float pause): Component(owner),
                                                                                              PaceSound(""),
                                                                                              PaceSpeed(speed),
                                                                                              MaxPaceDistance(maxDistance),
                                                                                              PauseTimer(pause),
                                                                                              Position(Parent.GetComponent<Components::Transform>()->Position)
            {
                DetectionDelay = 0.3;
                CurrentAction = Action::LEFT;
                NextAction = Action::PAUSE;
                Direction = 0;
                Moved = 0;
                timer = 0;
                playerDetected = false;

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
                ForLease->Dispatcher.Attach(NULL, this, "PlayerSeen", &EnemyPace::OnPlayerSeen, &Parent);
                ForLease->Dispatcher.Attach(NULL, this, "PlayerNotSeen", &EnemyPace::OnPlayerNotSeen, &Parent);
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
                case Action::WATCH:
                    WatchPlayer(dt);
                }
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
                timer += dt;
                if (timer >= PauseTimer)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::PAUSE;
                    timer = 0;
                    Components::Model* model = Parent.GetComponent<Components::Model>();
                    Components::VisionCone* vision = Parent.GetComponent<Components::VisionCone>();
                    if (model)
                        model->FlipY = !(model->FlipY);
                    if (vision)
                        vision->Direction[0] *= -1;
                }
            }

            void EnemyPace::WatchPlayer(float dt)
            {
                if (timer >= DetectionDelay)
                switch(NextAction)
                {
                case Action::LEFT:
                    MoveLeft(dt);
                    break;
                case Action::RIGHT:
                    MoveRight(dt);
                    break;
                }
                timer += dt;
            }

            void EnemyPace::OnPlayerSeen(Event const* e)
            {

            }

            void
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
