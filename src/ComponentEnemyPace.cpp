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
            EnemyPace::EnemyPace(Entity& owner, float speed, float maxDistance, float pause, unsigned direction): Component(owner),
                                                                                              PaceSound(""),
                                                                                              PaceSpeed(speed),
                                                                                              MaxPaceDistance(maxDistance),
                                                                                              PauseTimer(pause),
                                                                                              Position(Parent.GetComponent<Components::Transform>()->Position),
                                                                                              Direction(direction)
            {
                MaxPaceDistance = 3.0;
                DetectionDelay = 0.5;
                CurrentAction = Action::LEFT;
                NextAction = Action::PAUSE;
                //Direction = 0;
                Moved = 0;
                ptimer = 0;
                dtimer = 0;
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
                ForLease->Dispatcher.Detach(this, "PlayerSeen");
                ForLease->Dispatcher.Detach(this, "PlayerNotSeen");
            }

            void EnemyPace::Initialize()
            {
                ForLease->Dispatcher.Attach(NULL, this, "PlayerSeen", &EnemyPace::OnPlayerSeen, &Parent);
                ForLease->Dispatcher.Attach(NULL, this, "PlayerNotSeen", &EnemyPace::OnPlayerNotSeen, &Parent);
            }

            void EnemyPace::Update()
            {
                float dt = ForLease->FrameRateController().GetDt();
                if (playerDetected)
                    dtimer += dt;

                if (dtimer <= DetectionDelay)
                {
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
                }
            }


            void EnemyPace::MoveLeft(float dt)
            {
                Position[Direction] += -PaceSpeed * dt;
                Moved += PaceSpeed * dt;

                Components::Collision* collision = Parent.GetComponent<Components::Collision>();
                if (collision) {
                    std::vector<Entity*> entities = ForLease->GameStateManager().CurrentState().GetEntitiesInBox(Position, collision->ScaledWidth(), collision->ScaledHeight());
                    for (Entity* entity : entities) {
                        if (entity->HasComponent(ComponentType::Physics)) {
                            entity->GetComponent<Components::Physics>()->Velocity[Direction] = 0;
                            entity->GetComponent<Components::Transform>()->Position += LastMovement();
                        }
                    }
                }

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

                Components::Collision* collision = Parent.GetComponent<Components::Collision>();
                if (collision) {
                    std::vector<Entity*> entities = ForLease->GameStateManager().CurrentState().GetEntitiesInBox(Position, collision->ScaledWidth(), collision->ScaledHeight());
                    for (Entity* entity : entities) {
                        if (entity->HasComponent(ComponentType::Physics)) {
                            entity->GetComponent<Components::Physics>()->Velocity[Direction] = 0;
                            entity->GetComponent<Components::Transform>()->Position += LastMovement();
                        }
                    }
                }

                if (Moved >= MaxPaceDistance)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::LEFT;
                    Moved = -MaxPaceDistance;
                }
            }

            void EnemyPace::MovePause(float dt)
            {
                ptimer += dt;
                //Position[Direction] = 0;
                if (ptimer >= PauseTimer)
                {
                    CurrentAction = NextAction;
                    NextAction = Action::PAUSE;
                    ptimer = 0;
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
                if (ptimer >= DetectionDelay)
                    return;

                switch(NextAction)
                {
                case Action::LEFT:
                    MoveLeft(dt);
                    break;
                case Action::RIGHT:
                    MoveRight(dt);
                    break;
                }
                ptimer += dt;
            }

            void EnemyPace::OnPlayerSeen(Event const* e)
            {
                std::cout << "player seen\n";
                playerDetected = true;
            }

            void EnemyPace::OnPlayerNotSeen(Event const* e)
            {
                std::cout << "player not seen\n";
                playerDetected = false;
                dtimer = 0;
            }

            void EnemyPace::Serialize(Serializer& root)
            {
                root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
                Serializer pace = root.GetChild("EnemyPace");
                pace.WriteFloat("MaxPaceDistance", MaxPaceDistance);
                pace.WriteFloat("PaceSpeed", PaceSpeed);
                pace.WriteFloat("PauseTimer", PauseTimer);
                pace.WriteInt("Direction", Direction);
                pace.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
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

            Vector EnemyPace::LastMovement(bool scaleDt) {
                Vector movement(0,0);

                float dt;
                if (scaleDt)
                    dt = ForLease->FrameRateController().GetDt();
                else
                    dt = 1.0f;

                if (CurrentAction == Action::LEFT)
                    movement[Direction] = -(PaceSpeed * dt);
                else if (CurrentAction == Action::RIGHT)
                    movement[Direction] = (PaceSpeed * dt);

                return movement;
            }
    }
}
