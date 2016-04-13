/*!
    \author Sam Montanari

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentFinaleOne.h"
#include "Interpolation.h"
#include "State.h"
#include <iostream>
#include "Random.h"

namespace ForLeaseEngine
{
    namespace Components
    {
        FinaleOne::FinaleOne(Entity& owner) : Component(owner, ComponentType::Collision), Camera(""), Wisp(""), WispDest(""),
                                              CameraDest(""), Light(""), Eyes(""), Trigger(""), CameraSpeed(0),
                                              CameraSize(0), CameraGrowth(0), WispSpeed(0), PlayerMoveDistance(0),
                                              PlayerMoveSpeed(0), OverlaySpeed(0), LightGrowth(0), TransitionTimer(1),
                                              TransitionTimer2(1), WispLightTimer(1), start(false), distance(0),
                                              currentAction(Action::MOVE),setup(false), transitionDelay(0),
                                              transitionDelay2(0), wispLightDelay(0), lightSwitch(false)
        { }

        FinaleOne::~FinaleOne()
        {
            //ForLease->Dispatcher.Detach(this,"CollisionStarted");
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
            //std::cout << "registered\n";
            //ForLease->Dispatcher.Attach(NULL, this, "CollisionStarted", &FinaleOne::OnCollisionStart, &Parent);
        }

        void FinaleOne::OnCollisionStart(const Event* e)
        {
            const CollisionStartedEvent* collision = reinterpret_cast<const CollisionStartedEvent*>(e);
            std::cout << collision->With->GetName() << std::endl;
            if (!collision->With->GetName().compare(Trigger) && !start)
            {
                start = true;
                std::cout << "start shit\n";
            }

        }

        void FinaleOne::Update()
        {
            if (!start)
            {
                Components::Collision* parentCollision = Parent.GetComponent<Components::Collision>();
                start = parentCollision->CollidedWith(Trigger);
            }

            else if (start)
            {
                float dt = ForLease->FrameRateController().GetDt();
                wispLightDelay += dt;
                if (wispLightDelay >= WispLightTimer && !lightSwitch)
                {
                    Components::Light* light = ForLease->GameStateManager().CurrentState().GetEntityByName(Wisp)->GetComponent<Components::Light>();
                    light->Radius = 0;
                    light->LightColor.SetAll(1, 0, 0, 1);
                    lightSwitch = true;
                }

                switch(currentAction)
                {
                case Action::MOVE:
                    Move(dt);
                    break;
                case Action::TRANSITION:
                    Parent.GetComponent<Components::Transform>()->Position[0] = savePos;
                    Transition(dt);
                    break;
                }
            }
        }

        void FinaleOne::Move(float dt)
        {
            Entity* cam = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
            Entity* w = ForLease->GameStateManager().CurrentState().GetEntityByName(Wisp);
            float& wispSize = w->GetComponent<Components::ParticleEmitter>()->Size;
            float& size = cam->GetComponent<Components::Camera>()->Size;
            float& position = Parent.GetComponent<Components::Transform>()->Position[0];
            bool done1 = false, done2 = false;
            Components::Model* ParentModel = Parent.GetComponent<Components::Model>();
            if (!setup)
            {
                targetSize = size + CameraSize;
                ParentModel->SetAnimation(Parent.GetComponent<Components::CharacterController>()->WalkAnimation);
                ParentModel->FrameRate *= 0.5;
                Parent.DeleteComponent(ComponentType::PlayerController);
                Parent.GetComponent<Components::Physics>()->Velocity[0] = 0;
                Parent.GetComponent<Components::Physics>()->UnaffectedByGravity = false;
                Entity* wd = ForLease->GameStateManager().CurrentState().GetEntityByName(WispDest);
                Entity* cam = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
                Entity* cd = ForLease->GameStateManager().CurrentState().GetEntityByName(CameraDest);
                cameraPos = cd->GetComponent<Components::Transform>()->Position;
                Components::Follow* wispFollow = w->GetComponent<Components::Follow>();
                wispFollow->Active = true;
                //wispFollow->FollowBeginDistance = 0;
                //wispFollow->FollowEndDistance = 0;
                wispFollow->Speed = WispSpeed;
                wispFollow->Offset = Vector(0, 0);
                wispFollow->FollowEntityID = wd->GetID();
                Components::Follow* cameraFollow = cam->GetComponent<Components::Follow>();
                cameraFollow->Active = true;
                //cameraFollow->FollowBeginDistance = 0;
                //cameraFollow->FollowEndDistance = 0;
                cameraFollow->Speed = CameraSpeed;
                cameraFollow->Offset = Vector(0, 0);
                cameraFollow->FollowEntityID = cd->GetID();
                setup = true;
            }

            if(wispSize <= 5)
                wispSize += dt;

            if (size <= targetSize)
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
                savePos = position;
                done2 = true;
                ParentModel->SetAnimation("");
            }

            if (done1 && done2)
            {
                currentAction = Action::TRANSITION;
                setup = false;
            }
        }

        void FinaleOne::Transition(float dt)
        {
            Point& position = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera)->GetComponent<Components::Transform>()->Position;
            transitionDelay += dt;
            LevelComponents::Renderer* render = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();
            float& wispRadius = ForLease->GameStateManager().CurrentState().GetEntityByName(Wisp)->GetComponent<Components::Light>()->Radius;
            if (wispRadius <= 8)
                wispRadius += dt * 2;

            if (transitionDelay >= TransitionTimer)
            {
                transitionDelay2 += dt;

                if (!setup)
                {
                    render->SetOverlayColor(1, 1, 1, 0);
                    Components::Model* eyeModel = ForLease->GameStateManager().CurrentState().GetEntityByName(Eyes)->GetComponent<Components::Model>();
                    eyeModel->SetAnimation("ForestSpirit2AniEyeOpen.json");
                    eyeModel->AnimationActive = true;
                    eyeModel->Looping = false;
                    setup = true;
                }

                float& radius = ForLease->GameStateManager().CurrentState().GetEntityByName(Light)->GetComponent<Components::Light>()->Radius;
                float alpha = render->GetOverlayColor().GetA();
                radius += LightGrowth * dt;
                if (alpha <= 1 && transitionDelay2 >= TransitionTimer2)
                {
                    position = cameraPos + Vector(RandomFloat(-2, 2), RandomFloat(-2, 2));
                    alpha += OverlaySpeed * dt;
                    render->SetOverlayColor(1, 1, 1, alpha);
                }
            }

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
            finale.WriteFloat("TransitionTimer", TransitionTimer);
            finale.WriteFloat("TransitionTimer2", TransitionTimer2);
            finale.WriteFloat("WispLightTimer", WispLightTimer);

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
            finale.ReadFloat("TransitionTimer", TransitionTimer);
            finale.ReadFloat("TransitionTimer2", TransitionTimer2);
            finale.ReadFloat("WispLightTimer", WispLightTimer);
        }
    }
}
