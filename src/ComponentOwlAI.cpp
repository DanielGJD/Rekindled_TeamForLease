/*!
    \author Sam Montanari

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentOwlAI.h"
#include "ComponentsInclude.h"

namespace ForLeaseEngine
{
    namespace Components
    {
        OwlAI::OwlAI(Entity& owner, float blinkTimer, float watchTimer, Vector dir1, Vector dir2) : Component(owner, ComponentType::VisionCone),
                                                                                                    BlinkTime(blinkTimer),
                                                                                                    WatchTime(watchTimer),
                                                                                                    Direction1(dir1),
                                                                                                    Direction2(dir2),
                                                                                                    Animation("")
        {
            timer = 0;
            pause = false;
            currentState = STATE::LOOK;
            modelFlip = false;
            nextDirection = &Direction2;

            if ((Direction1[0] < 0 && Direction2[0] > 0) || (Direction1[0] > 0 && Direction2[0] < 0))
                modelFlip = true;
            else
                modelFlip = false;

        }

        OwlAI* OwlAI::Create(Entity& owner)
        {
            OwlAI* owl = new OwlAI(owner);
            owl->Initialize();
            return owl;
        }

        OwlAI::~OwlAI()
        {
            ForLease->Dispatcher.Detach(this, "ObjectSeen");
            ForLease->Dispatcher.Detach(this, "ObjectNotSeen");
        }

        void OwlAI::Initialize()
        {
            ForLease->Dispatcher.Attach(NULL, this, "ObjectSeen", &OwlAI::OnObjectSeen, &Parent);
            ForLease->Dispatcher.Attach(NULL, this, "ObjectNotSeen", &OwlAI::OnObjectNotSeen, &Parent);
            Components::VisionCone* vision = Parent.GetComponent<Components::VisionCone>();
            vision->Direction = Direction1;
        }

        void OwlAI::Update()
        {
            if (pause)
                return;

            float dt = ForLease->FrameRateController().GetDt();

            switch (currentState)
            {
            case STATE::LOOK:
                Look(dt);
                break;
            case STATE::BLINK:
                Blink(dt);
                break;
            }
        }

        void OwlAI::Look(float dt)
        {
            timer += dt;
            if (timer >= WatchTime)
            {
                Components::VisionCone* vision = Parent.GetComponent<Components::VisionCone>();
                vision->Direction = *nextDirection;
                vision->Active = false;
                vision->Visible = false;
                if (Parent.HasComponent(ComponentType::Model))
                {
                    Parent.GetComponent<Components::Model>()->SetAnimation(Animation);
                }

                currentState = STATE::BLINK;
                timer = 0;
                if (nextDirection == &Direction1)
                    nextDirection = &Direction2;

                else
                    nextDirection = &Direction1;
            }
        }

        void OwlAI::Blink(float dt)
        {
            timer += dt;
            if (timer >= BlinkTime)
            {
                currentState = STATE::LOOK;
                Components::VisionCone* vision = Parent.GetComponent<Components::VisionCone>();
                vision->Active = true;
                vision->Visible = true;
                if (Parent.HasComponent(ComponentType::Model))
                    Parent.GetComponent<Components::Model>()->SetAnimation("");
                timer = 0;

                if (modelFlip)
                {
                    Components::Model* model = Parent.GetComponent<Components::Model>();
                    vision->Offset[0] *= -1;

                    if (model)
                        model->FlipY = !(model->FlipY);
                }

            }
        }

        void OwlAI::OnObjectSeen(Event const* e)
        {
            pause = true;
        }

        void OwlAI::OnObjectNotSeen(Event const* e)
        {
            pause = false;
        }

        void OwlAI::Serialize(Serializer& root)
        {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer owl = root.GetChild("OwlAI");
            owl.WriteString("Animation", Animation);
            owl.WriteFloat("BlinkTime", BlinkTime);
            owl.WriteFloat("LookTime", WatchTime);
            owl.WriteVec("Direction1", Direction1);
            owl.WriteVec("Direction2", Direction2);
            owl.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            root.Append(owl, "OwlAI");
        }

        void OwlAI::Deserialize(Serializer& root)
        {
            Serializer owl = root.GetChild("OwlAI");
            owl.ReadString("Animation", Animation);
            owl.ReadFloat("BlinkTime", BlinkTime);
            owl.ReadFloat("LookTime", WatchTime);
            owl.ReadVec("Direction1", Direction1);
            owl.ReadVec("Direction2", Direction2);

            if ((Direction1[0] < 0 && Direction2[0] > 0) || (Direction1[0] > 0 && Direction2[0] < 0))
                modelFlip = true;
            else
                modelFlip = false;

            Components::VisionCone* vision = Parent.GetComponent<Components::VisionCone>();
            vision->Direction = Direction1;

            if (Parent.HasComponent(ComponentType::Model))
            {
                Parent.GetComponent<Components::Model>()->AnimationActive = true;
            }

        }
    }
}
