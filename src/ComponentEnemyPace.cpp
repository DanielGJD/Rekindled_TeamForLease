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
            MaxPaceDistance = 3.0;
            DetectionDelay = 0.5;
            ResumeTime = 2.0;
            CurrentAction = Action::LEFT;
            NextAction = Action::PAUSE;
            Direction = 0;
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
            ForLease->Dispatcher.Detach(this, "ObjectSeen");
            ForLease->Dispatcher.Detach(this, "ObjectNotSeen");
        }

        void EnemyPace::Initialize()
        {
            ForLease->Dispatcher.Attach(NULL, this, "ObjectSeen", &EnemyPace::OnPlayerSeen, &Parent);
            ForLease->Dispatcher.Attach(NULL, this, "ObjectNotSeen", &EnemyPace::OnPlayerNotSeen, &Parent);
        }

        void EnemyPace::Update()
        {
            float dt = ForLease->FrameRateController().GetDt();
            if (playerDetected)
                dtimer += dt;
            else
                rtimer += dt;

            if ((dtimer <= DetectionDelay && playerDetected) ||
                (rtimer > ResumeTime && !playerDetected))
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
            ptimer += dt;
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
            //rtimer = 0;
        }

        void EnemyPace::OnPlayerNotSeen(Event const* e)
        {
            std::cout << "player not seen\n";
            playerDetected = false;
            dtimer = 0;
            rtimer = 0;
        }

        void EnemyPace::Serialize(Serializer& root)
        {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer pace = root.GetChild("EnemyPace");
            pace.WriteFloat("MaxPaceDistance", MaxPaceDistance);
            pace.WriteFloat("PaceSpeed", PaceSpeed);
            pace.WriteFloat("PauseTimer", PauseTimer);
            pace.WriteFloat("ResumeTime", ResumeTime);
            pace.WriteInt("Direction", Direction);
            pace.WriteInt("CurrentAction", CurrentAction);
            pace.WriteInt("NextAction", NextAction);
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
            pace.ReadFloat("ResumeTime", ResumeTime);
            pace.ReadInt("CurrentAction", CurrentAction);
            pace.ReadInt("NextAction", NextAction);
            pace.ReadInt("Direction", Direction);
            rtimer = ResumeTime;
        }
    }
}
