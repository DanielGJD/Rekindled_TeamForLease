#include "ComponentFinaleOne.h"
#include "Interpolation.h"
namespace ForLeaseEngine
{
    namespace Components
    {
        FinaleOne::FinaleOne(Entity& owner) : Component(owner), Camera(""), Wisp(""), WispDest(""),
                                              CameraDest(""), Light(""), Eyes(""), Trigger(""), CameraSpeed(0),
                                              CameraSize(0), CameraGrowth(0), WispSpeed(0), PlayerMoveDistance(0),
                                              PlayerMoveSpeed(0), OverlaySpeed(0), LightGrowth(0), start(false),
                                              distance(0), currentAction(Action::MOVE), setup(false)
        { }

        FinaleOne::~FinaleOne()
        {
            ForLease->Dispatcher->Detach(this,"CollisionStarted");
        }

        ComponentType FinaleOne::GetType()
        {
            return Type;
        }

        FinaleOne* FinaleOne::Create(Entity& owner)
        {
            FinaleOne* finale = new FinaleOne(owner);
            finale->Initialize();
            return finale;
        }
        void FinaleOne::Initialize()
        {
            ForLease->Dispatcher.Attach(NULL, this, "CollisionStarted", &FinaleOne::OnCollisionStart, &Parent);
        }

        void FinaleOne::OnCollisionStart(const Event* e)
        {
            const CollisionStartedEvent* collision = reinterpret_cast<const CollisionStartedEvent*>(e);
            if (collision->With->GetName() == Trigger && !start)
                start = true;
        }

        void FinaleOne::Update()
        {
            if (start)
            {
                float dt = ForLease->FrameRateController().GetDt();

                switch(currentAction)
                {
                case Action::MOVE:
                    Move(dt);
                    break;
                case Action::TRANSITION:
                    Transition(dt);
                    break;
                }
            }
        }

        void FinaleOne::Move(float dt)
        {
            Entity* cam = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
            float& size = cam->GetComponent<Components::Camera>()->Size;
            float& position = Parent.GetComponent<Components::Transform>()->Position[0];
            bool done1 = false, done2 = false;
            Components::Model* ParentModel = Parent.GetComponent<Components::Model>();

            if (!setup)
            {
                ParentModel->SetAnimation(Parent.GetComponent<Components::CharacterController>()->WalkAnimation);
                Parent.DeleteComponent(ComponentType::CharacterController);
                Entity* w = ForLease->GameStateManager().CurrentState().GetEntityByName(Wisp);
                Entity* wd = ForLease->GameStateManager().CurrentState().GetEntityByName(WispDest);
                Entity* cam = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
                Entity* cd = ForLease->GameStateManager().CurrentState().GetEntityByName(CameraDest);
                Components::Follow* wispFollow = w->GetComponent<Components::Follow>();
                wispFollow->Active = true;
                wispFollow->FollowBeginDistance = 0;
                wispFollow->FollowEndDistance = 0;
                wispFollow->Speed = WispSpeed;
                wispFollow->Offset = Vector(0, 0);
                wispFollow->FollowEntityID = wd->GetID();
                Components::Follow* cameraFollow = cam->GetComponent<Components::Follow>();
                cameraFollow->Active = true;
                cameraFollow->FollowBeginDistance = 0;
                cameraFollow->FollowEndDistance = 0;
                cameraFollow->Speed = CameraSpeed;
                cameraFollow->Offset = Vector(0, 0);
                cameraFollow->FollowEntityID = cd->GetID();
                setup = true;
            }

            if (size <= CameraSize)
                size += CameraGrowth * dt;
            else
                done1 = true;

            if (distance <= PlayerMoveDistance)
            {
                position += PlayerMoveSpeed * dt;
                distance += PlayerMoveSpeed * dt;
            }
            else
            {
                done2 = true;
                ParentModel->SetAnimation("");
            }

            if (done1 && done2)
                currentAction = Action::TRANSITION;

        }

        void FinaleOne::Serialize(Serializer& root)
        {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer finale = root.GetChild("FinaleOne");

            finale.WriteString("Camera", Camera);
            finale.WriteString("Wisp", Wisp);
            finale.WriteString("WispDest", WispDest);
            finale.WriteString("CameraDest", CameraDest);
            finale.WriteString("Light", Light);
            finale.WriteString("Eyes", Eyes);
            finale.WriteString("Trigger", Trigger);

            finale.WriteFloat("CameraSpeed", CameraSpeed);
            finale.WriteFloat("CameraSize", CameraSize);
            finale.WriteFloat("CameraGrowth", CameraGrowth);
            finale.WriteFloat("WispSpeed", WispSpeed);
            finale.WriteFloat("PlayerMoveDistance", PlayerMoveDistance);
            finale.WriteFloat("PlayerMoveSpeed", PlayerMoveSpeed);
            finale.WriteFloat("OverlaySpeed", OverlaySpeed);
            finale.WriteFloat("LightGrowth", LightGrowth);

            root.Append(finale, "FinaleOne");
        }

        void FinaleOne::Deserialize(Serializer& root)
        {
            Serializer finale = root.GetChild("FinaleOne");

            finale.ReadString("Camera", Camera);
            finale.ReadString("Wisp", Wisp);
            finale.ReadString("WispDest", WispDest);
            finale.ReadString("CameraDest", CameraDest);
            finale.ReadString("Light", Light);
            finale.ReadString("Eyes", Eyes);
            finale.ReadString("Trigger", Trigger);

            finale.ReadFloat("CameraSpeed", CameraSpeed);
            finale.ReadFloat("CameraSize", CameraSize);
            finale.ReadFloat("CameraGrowth", CameraGrowth);
            finale.ReadFloat("WispSpeed", WispSpeed);
            finale.ReadFloat("PlayerMoveDistance", PlayerMoveDistance);
            finale.ReadFloat("PlayerMoveSpeed", PlayerMoveSpeed);
            finale.ReadFloat("OverlaySpeed", OverlaySpeed);
            finale.ReadFloat("LightGrowth", LightGrowth);
        }
    }
}
