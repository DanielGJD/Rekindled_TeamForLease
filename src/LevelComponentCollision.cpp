/*!
    \file   LevelComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision system.
    \see LevelComponentCollision.h

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentCollision.h"
#include "ComponentsInclude.h"
#include "Component.h"
#include <cmath>
#include "EventDispatcher.h"
#include "Engine.h"
#include "CollisionEvent.h"
#include "HalfPlane.h"
#include "Ray.h"
#include "LevelComponentRenderer.h"
#include "HalfPlane.h"
#include <limits>
#include <algorithm>
#include <iterator>

int i = 0;

namespace ForLeaseEngine {

    namespace LevelComponents {

        /*!
            Constructor for a new Collision LevelComponent.

            \param owner
                The State instance that created this Collision LevelComponent.
        */
        Collision::Collision(State& owner, float vertexSpacing) : LevelComponent(owner, ComponentType::Collision), VertexSpacing(vertexSpacing) {}

        /*!
            Update function.  Loops through all entities and checks collisions.

            \param entities
                A reference to a vector of Entity pointers that we want to check
                for collisions on.

            \bug This is not well done.  Complexity O(n^2).  This will be reworked
            after Engine Proof, and I'll include mesh vertex collision checking
            then.
        */
        void Collision::Update(std::vector<Entity *>& entities) {
            std::cout << "Collision Update" << std::endl;
            std::vector<Entity *> collisionEntities;
            std::vector<Entity *> movingPlatforms;
            std::vector<Entity *> physicsEntities;

            // Pre-screen entities for the collision component
            for (Entity* entity : entities) {
                if (entity->HasComponent(ComponentType::Collision)) {
                    entity->GetComponent<Components::Collision>()->CollidedLastFrame = false;
                    collisionEntities.push_back(entity);
                }

                if (entity->HasComponent(ComponentType::Physics) && !entity->HasComponent(ComponentType::MovingPlatform))
                    physicsEntities.push_back(entity);

                if (entity->HasComponent(ComponentType::MovingPlatform))
                    movingPlatforms.push_back(entity);
            }

            //for (Entity* entity : physicsEntities) {
            //    for (Entity* other : entity->GetComponent<Components::Collision>()->)
            //}

            //for (Entity* entity : movingPlatforms) {
            //    PhysicsCompute(entity);
            //    PhysicsCleanup(entity);
            //    //ResolveIndividualSweptCollision(entity, SweptCollision(), 0.0f);
            //    CheckAndResolveMovingPlatformSweptCollisions(entity, physicsEntities, collisionEntities);
            //    if (entity->GetComponent<Components::MovingPlatform>()->CurrentAction == Components::MovingPlatform::Action::Away
            //        && entity->GetComponent<Components::MovingPlatform>()->Direction == Components::MovingPlatform::Axis::Vertical) {
            //        for (Entity* touching : entity->GetComponent<Components::Collision>()->CollidedWithLastFrame) {
            //            touching->GetComponent<Components::Transform>()->Position += entity->GetComponent<Components::MovingPlatform>()->LastMovement();
            //        }
            //    }
            //}

            for (Entity* entity : movingPlatforms) {
                UpdateMovingPlatform(entity, physicsEntities);
            }

            for (Entity* entity : physicsEntities) {
                PhysicsCompute(entity);
                PhysicsCleanup(entity);
                CheckAndResolveSweptCollisions(entity, collisionEntities);
            }

            /*for (Entity* entity1 : entities) {
                if (!CheckEntityCompatibility(entity1)) continue;

                for (Entity* entity2 : entities) {
                    if (entity2 == entity1 || !CheckEntityCompatibility(entity2)) continue;

                    if (CheckCollision(entity1, entity2)) {
                        CollisionEvent e1(entity2);
                        CollisionEvent e2(entity1);
                        ForLease->Dispatcher.DispatchToParent(&e1, entity1);
                        ForLease->Dispatcher.DispatchToParent(&e2, entity2);
                        ResolveCollision(entity1, entity2);
                    }

                }
            }*/
        }

        void Collision::UpdateMovingPlatform(Entity* entity, std::vector<Entity*> physicsEntities) {
            Components::Transform* transform = entity->GetComponent<Components::Transform>();
            Components::Collision* collision = entity->GetComponent<Components::Collision>();
            Components::Physics* physics = entity->GetComponent<Components::Physics>();
            Components::MovingPlatform* platform = entity->GetComponent<Components::MovingPlatform>();
            float dt = ForLease->FrameRateController().GetDt();

            std::vector<Entity*> allAffected;

            for (Entity* check : physicsEntities) {
                if (IsAffectedByMovingPlatform(entity, check))
                    allAffected.push_back(check);
            }

            Vector movement = physics->Velocity * dt;
            transform->Position += movement;

            if (platform->Direction == Components::MovingPlatform::Axis::Horizontal) {
                for (Entity* affected : allAffected) {
                    affected->GetComponent<Components::Transform>()->Position += movement;
                }
            } else {
                for (Entity* affected : allAffected) {
                    Components::Transform* aTransform = affected->GetComponent<Components::Transform>();
                    Components::Collision* aCollision = affected->GetComponent<Components::Collision>();

                    aTransform->Position.y = transform->Position.y + collision->ScaledHalfHeight() + aCollision->ScaledHalfHeight() + Epsilon;
                }
            }

            for (Entity* toPush : physicsEntities) {
                if (toPush->HasComponent(ComponentType::Collision)) {
                    MovingPlatformPushOut(entity, toPush);
                }
            }
        }

        void Collision::MovingPlatformPushOut(Entity* platform, Entity* toPush) {
            Components::MovingPlatform* pPlatform = platform->GetComponent<Components::MovingPlatform>();
            Components::Collision* pCollision = platform->GetComponent<Components::Collision>();
            Components::Collision* tCollision = toPush->GetComponent<Components::Collision>();

            //if (!BoxesIntersect(pCollision->BotRight(), pCollision->TopLeft(), tCollision->BotRight(), tCollision->TopLeft()))
            //    return;

            //if (pPlatform->Direction == Components::MovingPlatform::Axis::Horizontal)
            //    HorizontalMovingPlatformPushOut(platform, toPush);

        }

        void Collision::HorizontalMovingPlatformPushOut(Entity* platform, Entity* toPush) {
            Components::MovingPlatform* pPlatform = platform->GetComponent<Components::MovingPlatform>();
            Components::Collision* pCollision = platform->GetComponent<Components::Collision>();

            Components::Transform* tTransform = toPush->GetComponent<Components::Transform>();
            Components::Collision* tCollision = toPush->GetComponent<Components::Collision>();

            CollisionSide side = CollisionSide::None;
            float distance = std::numeric_limits<float>::infinity();

            float leftDistance = std::abs(pCollision->TopLeft().x - tCollision->TopRight().x);
            if (leftDistance >= 0 && leftDistance < distance) {
                distance = leftDistance;
                side = CollisionSide::Left;
            }

            float rightDistance = std::abs(pCollision->TopRight().x - tCollision->TopLeft().x);
            if (rightDistance >= 0 && rightDistance < distance) {
                distance = rightDistance;
                side = CollisionSide::Right;
            }

            if (side == CollisionSide::Left) {
                tTransform->Position.x -= (distance + Epsilon);
            } else if (side == CollisionSide::Right) {
                tTransform->Position.x += distance + Epsilon;
            }
        }

        void Collision::VerticalMovingPlatformPushOut(Entity* platform, Entity* toPush) {
            Components::MovingPlatform* pPlatform = platform->GetComponent<Components::MovingPlatform>();
            Components::Collision* pCollision = platform->GetComponent<Components::Collision>();

            Components::Transform* tTransform = toPush->GetComponent<Components::Transform>();
            Components::Collision* tCollision = toPush->GetComponent<Components::Collision>();

            CollisionSide side = CollisionSide::None;
            float distance = std::numeric_limits<float>::infinity();

            float topDistance = pCollision->TopRight().y - tCollision->TopRight().y;
            if (topDistance > 0 && topDistance < distance) {
                distance = topDistance;
                side = CollisionSide::Top;
            }

            if (side == CollisionSide::Top) {
                tTransform->Position.y += distance + Epsilon;
            }
        }

        bool Collision::IsAffectedByMovingPlatform(Entity* platform, Entity* check) {
            Components::MovingPlatform::Axis direction = platform->GetComponent<Components::MovingPlatform>()->Direction;
            if (direction == Components::MovingPlatform::Axis::Horizontal) {
                return IsAffectedByHoriziontalMovingPlatform(platform, check);
            } else if (direction == Components::MovingPlatform::Axis::Vertical) {
                return IsAffectedByVerticalMovingPlatform(platform, check);
            }
        }

        bool Collision::IsAffectedByHoriziontalMovingPlatform(Entity* platform, Entity* check) {
            Components::Collision* pCollision = platform->GetComponent<Components::Collision>();
            Components::MovingPlatform* pPlatform = platform->GetComponent<Components::MovingPlatform>();
            Point pbr = pCollision->TopRight();
            Point ptl = pCollision->TopLeft();
            ptl.y += pPlatform->AffectedFieldHeight;

            Components::Collision* cCollision = check->GetComponent<Components::Collision>();
            Point cbr = cCollision->BotRight();
            Point ctl = cCollision->BotLeft();
            ctl.y += pPlatform->AffectedFieldHeight;

            if (!cCollision) return false;
            else             return BoxesIntersect(pbr, ptl, cbr, ctl);
        }

        bool Collision::IsAffectedByVerticalMovingPlatform(Entity* platform, Entity* check) {
            Components::Collision* pCollision = platform->GetComponent<Components::Collision>();
            Components::Physics* pPhysics = platform->GetComponent<Components::Physics>();
            Components::MovingPlatform* pPlatform = platform->GetComponent<Components::MovingPlatform>();
            Point pbr = pCollision->TopRight();
            Point ptl = pCollision->TopLeft();
            ptl.y += pPlatform->AffectedFieldHeight;
            if (pPhysics->Velocity.y > 0) ptl.y += pPhysics->Velocity.y * ForLease->FrameRateController().GetDt();

            Components::Collision* cCollision = check->GetComponent<Components::Collision>();
            Point cbr = cCollision->BotRight();
            Point ctl = cCollision->BotLeft();
            ctl.y += pPlatform->AffectedFieldHeight;

            if (!cCollision) return false;
            else             return BoxesIntersect(pbr, ptl, cbr, ctl);
        }

        /*!
            Takes four points--each box (1 and 2) has a bottom right and top left
            point.
        */
        bool Collision::BoxesIntersect(Point br1, Point tl1, Point br2, Point tl2) {
            return !(br1.x < tl2.x || tl1.x > br2.x || br1.y > tl2.y || tl1.y < br2.y);
        }

        void PostfixCollision(Entity* entity, Entity* collidedAgainst) {
            //std::cout << "Do stuff here.";
            Components::Transform* eTransform = entity->GetComponent<Components::Transform>();
            Components::Collision* eCollision = entity->GetComponent<Components::Collision>();
            Components::Transform* cTransform = collidedAgainst->GetComponent<Components::Transform>();
            Components::Collision* cCollision = collidedAgainst->GetComponent<Components::Collision>();

            float xDist, yDist;

            if (eTransform->Position.x > cTransform->Position.x) xDist = eTransform->Position.x - cTransform->Position.x;
            else xDist = cTransform->Position.x - eTransform->Position.x;
            xDist = std::abs(xDist);

            if (eTransform->Position.y > cTransform->Position.y) yDist = eTransform->Position.y - cTransform->Position.y;
            else yDist = cTransform->Position.y - eTransform->Position.y;
            yDist = std::abs(yDist);



            xDist -= (eCollision->ScaledHalfWidth() + cCollision->ScaledHalfWidth());
            yDist -= (eCollision->ScaledHalfHeight() + cCollision->ScaledHalfHeight());

            if (xDist < 0 && xDist > yDist) {
                if (eTransform->Position.x > cTransform->Position.x) eTransform->Position.x += Epsilon;
                else eTransform->Position.x -= Epsilon;
            }
            else if (yDist < 0 && yDist > xDist) {
                if (eTransform->Position.y > cTransform->Position.y) eTransform->Position.y += Epsilon;
                else eTransform->Position.y -= Epsilon;
            }

            //eTransform->Position.y += Epsilon;

            //std::cout << eTransform->Position << " " << cTransform->Position << std::endl;
            //std::cout << xDist << " " << yDist << std::endl;
        }

        void Collision::CheckAndResolveSweptCollisions(Entity* entity, std::vector<Entity *>& entities) {
            float time = 1.0f;

            Components::Transform* transform = entity->GetComponent<Components::Transform>();
            Components::Collision* collision = entity->GetComponent<Components::Collision>();
            Components::Physics* physics = entity->GetComponent<Components::Physics>();
            std::unordered_set<Entity *> collidedWithThisFrame;
            std::vector<SweptCollision> collisionsThisFrame;

            while (time > 0.0f) {
                Entity* collidedAgainst = 0;
                SweptCollision firstCollision(Vector(0,0), time, CollisionSide::None);

                for (Entity* checkAgainst : entities) {
                    if (entity == checkAgainst) continue;
                    if (!BroadphaseSweptCollision(entity, checkAgainst, time)) continue;

                    if (!checkAgainst->GetComponent<Components::Collision>()->ResolveCollisions) {
                        CollisionEvent toCA = CollisionEvent(entity);
                        CollisionEvent toEntity = CollisionEvent(checkAgainst);
                        ForLease->Dispatcher.DispatchToParent(&toCA, checkAgainst);
                        ForLease->Dispatcher.DispatchToParent(&toEntity, entity);

                        if (collision->CollidedWithLastFrame.find(checkAgainst) == collision->CollidedWithLastFrame.end()) {
                            CollisionStartedEvent toCAStart = CollisionStartedEvent(entity, CollisionSide::None);
                            CollisionStartedEvent toEntityStart = CollisionStartedEvent(checkAgainst, CollisionSide::None);
                            ForLease->Dispatcher.DispatchToParent(&toCAStart, checkAgainst);
                            ForLease->Dispatcher.DispatchToParent(&toEntityStart, entity);
                        }

                        collidedWithThisFrame.insert(checkAgainst);

                        continue;
                    }

                    SweptCollision newCollision = CheckIndividualSweptCollision(entity, checkAgainst, time);
                    if (newCollision.Distance < firstCollision.Distance) {
                        // HACKY SHIT
                        // Makes sure that moving platforms only register on top-bottom collisions
                        if (checkAgainst->HasComponent(ComponentType::MovingPlatform)) {
                            if (newCollision.SelfSide == CollisionSide::Bottom) {
                                firstCollision = newCollision;
                                collidedAgainst = checkAgainst;
                            }
                        } else {
                            firstCollision = newCollision;
                            collidedAgainst = checkAgainst;
                        }
                    }
                }

                if (collidedAgainst) {
                    CollisionEvent toCA = CollisionEvent(entity, firstCollision.SelfSide);
                    CollisionEvent toEntity = CollisionEvent(collidedAgainst, firstCollision.Side);
                    ForLease->Dispatcher.DispatchToParent(&toCA, collidedAgainst);
                    ForLease->Dispatcher.DispatchToParent(&toEntity, entity);
                    collision->CollidedLastFrame = true;
                    collidedWithThisFrame.insert(collidedAgainst);
                    //collision->CollidedWith = collidedAgainst;

                    //if (collidedAgainst->HasComponent(ComponentType::MovingPlatform)
                    //    && firstCollision.SelfSide == CollisionSide::Bottom
                    //    && firstCollision.Side == CollisionSide::Top) {

                    //    Components::MovingPlatform* mp = collidedAgainst->GetComponent<Components::MovingPlatform>();

                    //    if (mp->Direction == Components::MovingPlatform::Axis::Horizontal) {
                    //        std::cout << ForLease->FrameRateController().FrameNumber << std::endl;
                    //        entity->GetComponent<Components::Transform>()->Position += collidedAgainst->GetComponent<Components::MovingPlatform>()->LastMovement();
                    //    }
                    //}

                    auto collidedLast = collision->CollidedWithLastFrame.find(collidedAgainst);
                    if (collidedLast == collision->CollidedWithLastFrame.end()) {
                        CollisionStartedEvent toCA = CollisionStartedEvent(entity, firstCollision.SelfSide);
                        CollisionStartedEvent toEntity = CollisionStartedEvent(collidedAgainst, firstCollision.Side);
                        ForLease->Dispatcher.DispatchToParent(&toCA, collidedAgainst);
                        ForLease->Dispatcher.DispatchToParent(&toEntity, entity);
                    }
                }

                time -= firstCollision.Distance;

                //if (collidedAgainst && collidedAgainst->GetComponent<Components::Collision>()->ResolveCollisions)
                ResolveIndividualSweptCollision(entity, firstCollision, time);

                //if (collidedAgainst && BoxesIntersect(collision->BotRight(), collision->TopLeft(), collidedAgainst->GetComponent<Components::Collision>()->BotRight(), collidedAgainst->GetComponent<Components::Collision>()->TopLeft()))
                //    PostfixCollision(entity, collidedAgainst);

                if (firstCollision.Side != CollisionSide::None)
                    collision->CollidedWithSide = firstCollision.Side;
            }

            //for (Entity* caEntity : entities) {
            //    if (caEntity == entity) continue;
            //    Components::Collision* caCollision = caEntity->GetComponent<Components::Collision>();
            //    if (!caCollision->ResolveCollisions) continue;
            //
            //    if (BoxesIntersect(collision->BotRight(), collision->TopLeft(), caCollision->BotRight(), caCollision->TopLeft()))
            //        PostfixCollision(entity, caEntity);
            //}

            //std::vector<Entity *> collisionStarted;
            std::vector<Entity *> collisionEnded;

            //for (Entity* collided : collidedWithThisFrame) {
            //    auto collidedLast = collision->CollidedWithLastFrame.find(collided);
            //    if (collidedLast == collision->CollidedWithLastFrame.end())
            //        collisionStarted.push_back(collided);
            //}

            for (Entity* collidedLast : collision->CollidedWithLastFrame) {
                auto collided = collidedWithThisFrame.find(collidedLast);
                if (collided == collidedWithThisFrame.end())
                    collisionEnded.push_back(collidedLast);
            }

            //for (Entity* collided : collisionStarted) {
                //CollisionStartedEvent toCA = CollisionStartedEvent(entity);
                //CollisionStartedEvent toEntity = CollisionStartedEvent(collided);
                //ForLease->Dispatcher.DispatchToParent(&toCA, collided);
                //ForLease->Dispatcher.DispatchToParent(&toEntity, entity);
                //collided->GetComponent<Components::Collision>()->CollidedWithLastFrame.insert(entity);
            //}

            for (Entity* collided : collisionEnded) {
                CollisionEndedEvent toCA = CollisionEndedEvent(entity);
                CollisionEndedEvent toEntity = CollisionEndedEvent(collided);
                ForLease->Dispatcher.DispatchToParent(&toEntity, entity);
                if (Owner.EntityExists(collided)) {
                    ForLease->Dispatcher.DispatchToParent(&toCA, collided);
                    collided->GetComponent<Components::Collision>()->CollidedWithLastFrame.erase(entity);
                }
            }

            collision->CollidedWithLastFrame = collidedWithThisFrame;
        }

        void Collision::CheckAndResolveMovingPlatformSweptCollisions(Entity* entity, std::vector<Entity *>& entities, std::vector<Entity *>& collisionEntities) {
            Components::Transform* transform = entity->GetComponent<Components::Transform>();
            Components::Collision* collision = entity->GetComponent<Components::Collision>();
            Components::Physics* physics = entity->GetComponent<Components::Physics>();
            Components::MovingPlatform* platform = entity->GetComponent<Components::MovingPlatform>();
            float dt = ForLease->FrameRateController().GetDt();

            //if (platform->Direction == Components::MovingPlatform::Axis::Vertical
            //    && platform->CurrentAction == Components::MovingPlatform::Action::Away) {
            //    ResolveIndividualSweptCollision(entity, SweptCollision(), 0.0f);
            //    return;
            //} else {
            //    ResolveIndividualSweptCollision(entity, SweptCollision(), 0.0f);
            //    return;
            //}

            float time = 1.0f;
            std::unordered_set<Entity *> collidedWithThisFrame;
            std::unordered_set<Entity *> ignore;

            while (time > Epsilon) {
                Entity* collidedAgainst = 0;
                SweptCollision firstCollision(Vector(0, 0), 1.0f, CollisionSide::None);

                for (Entity* checkAgainst : entities) {
                    if (entity == checkAgainst) continue;
                    if (!BroadphaseSweptCollision(entity, checkAgainst, time)) continue;

                    if (!checkAgainst->GetComponent<Components::Collision>()->ResolveCollisions) {
                        CollisionEvent toCA = CollisionEvent(entity);
                        CollisionEvent toEntity = CollisionEvent(checkAgainst);
                        ForLease->Dispatcher.DispatchToParent(&toCA, checkAgainst);
                        ForLease->Dispatcher.DispatchToParent(&toEntity, entity);
                        collidedWithThisFrame.insert(checkAgainst);
                        continue;
                    }

                    SweptCollision newCollision = CheckIndividualSweptCollision(entity, checkAgainst, time);
                    if (newCollision.Distance < firstCollision.Distance && ignore.find(checkAgainst) == ignore.end()) {
                        firstCollision = newCollision;
                        collidedAgainst = checkAgainst;
                    }
                }

                if (collidedAgainst) {
                    //CollisionEvent toCA = CollisionEvent(entity, firstCollision.SelfSide);
                    //CollisionEvent toEntity = CollisionEvent(collidedAgainst, firstCollision.Side);
                    //ForLease->Dispatcher.DispatchToParent(&toCA, collidedAgainst);
                    //ForLease->Dispatcher.DispatchToParent(&toEntity, entity);
                    collision->CollidedLastFrame = true;
                    collidedWithThisFrame.insert(collidedAgainst);
                    //collision->CollidedWith = collidedAgainst;
                    bool broadphase = false;
                    Vector movement = physics->Velocity * dt * 1.000001f;

                    //for (Entity* possible : collisionEntities) {
                    //    if (possible == collidedAgainst) continue;
                    //    if (BroadphaseSweptCollision(collidedAgainst, movement, possible)) {
                    //        broadphase = true;
                    //        ignore.insert(collidedAgainst);
                    //        break;
                    //    }
                    //}

                    //if (!broadphase) {
                        ResolveIndividualSweptCollision(collidedAgainst, movement, SweptCollision(Vector(0,0), time, CollisionSide::None));
                    //}
                }

                time -= firstCollision.Distance;

                //if (collidedAgainst && collidedAgainst->GetComponent<Components::Collision>()->ResolveCollisions)
            }
            ResolveIndividualSweptCollision(entity, SweptCollision(), 1.0f);
        }

        bool Collision::BroadphaseSweptCollision(Entity* resolve, Entity* against, float remainingTime) {
            Components::Physics* rPhysics = resolve->GetComponent<Components::Physics>();
            float dt = ForLease->FrameRateController().GetDt();
            Vector rVelocity = rPhysics->Velocity * dt * remainingTime;

            return BroadphaseSweptCollision(resolve, rVelocity, against);
        }

        bool Collision::BroadphaseSweptCollision(Entity* resolve, Vector movement, Entity* against) {
            Components::Collision* rCollision = resolve->GetComponent<Components::Collision>();
            Components::Collision* aCollision = against->GetComponent<Components::Collision>();
            float dt = ForLease->FrameRateController().GetDt();
            Vector rVelocity = movement;

            Point bpPositionTL = rCollision->TopLeft();
            Point bpPositionBR = rCollision->BotRight();

            if (rVelocity.x < 0) bpPositionTL.x += rVelocity.x;
            else bpPositionBR.x += rVelocity.x;

            if (rVelocity.y < 0) bpPositionBR.y += rVelocity.y;
            else bpPositionTL.y += rVelocity.y;

            Point aPositionTL = aCollision->TopLeft();
            Point aPositionBR = aCollision->BotRight();

            return BoxesIntersect(bpPositionBR, bpPositionTL, aPositionBR, aPositionTL);
            //!(bpPositionBR.x < aPositionTL.x || bpPositionTL.x > aPositionBR.x || bpPositionBR.y > aPositionTL.y || bpPositionTL.y < aPositionBR.y);
        }

        /*!
            Adapted from http://www.gamedev.net/page/resources/_/technical/game-programming/swept-aabb-collision-detection-and-response-r3084
        */
        SweptCollision Collision::CheckIndividualSweptCollision(Entity* resolve, Entity* against, float remainingTime) {
            Components::Transform* rTransform = resolve->GetComponent<Components::Transform>();
            Components::Collision* rCollision = resolve->GetComponent<Components::Collision>();
            Components::Physics* rPhysics = resolve->GetComponent<Components::Physics>();
            Vector rVelocity = rPhysics->Velocity * ForLease->FrameRateController().GetDt();

            Components::Transform* aTransform = against->GetComponent<Components::Transform>();
            Components::Collision* aCollision = against->GetComponent<Components::Collision>();

            // Get distances of edges
            Vector inverseEntry;
            Vector inverseExit;

            if (rVelocity.x > 0) {
                inverseEntry.x = aCollision->TopLeft().x - rCollision->TopRight().x;
                inverseExit.x = aCollision->TopRight().x - rCollision->TopLeft().x;
            } else {
                inverseEntry.x = aCollision->TopRight().x - rCollision->TopLeft().x;
                inverseExit.x = aCollision->TopLeft().x - rCollision->TopRight().x;
            }

            if (rVelocity.y < 0)
            {
                inverseEntry.y = aCollision->TopLeft().y - rCollision->BotRight().y;
                inverseExit.y = aCollision->BotRight().y - rCollision->TopLeft().y;
            }
            else
            {
                inverseEntry.y = aCollision->BotRight().y - rCollision->TopLeft().y;
                inverseExit.y = aCollision->TopLeft().y - rCollision->BotRight().y;
            }

            // Get entry/exit values
            Vector entry;
            Vector exit;

            if (rVelocity.x == 0.0f)
            {
                entry.x = -std::numeric_limits<float>::infinity();
                exit.x = std::numeric_limits<float>::infinity();
            }
            else
            {
                entry.x = inverseEntry.x / rVelocity.x;
                exit.x = inverseExit.x / rVelocity.x;
            }

            if (rVelocity.y == 0.0f)
            {
                entry.y = -std::numeric_limits<float>::infinity();
                exit.y = std::numeric_limits<float>::infinity();
            }
            else
            {
                entry.y = inverseEntry.y / rVelocity.y;
                exit.y = inverseExit.y / rVelocity.y;
            }

            float entryTime = std::max(entry.x, entry.y);
            float exitTime = std::min(exit.x, exit.y);

            Vector normal(0, 0);
            float dist = 1.0f;
            CollisionSide side = CollisionSide::None;

            // No collision
            if (entryTime > exitTime || (entry.x < 0.0f && entry.y < 0.0f) || (entry.x > 1.0f || entry.y > 1.0f))
                return SweptCollision(normal, 1, side);

            if (entry.x > entry.y) {
                if (inverseEntry.x < 0) {
                    normal = Vector(1, 0);
                    side = CollisionSide::Right;
                } else {
                    normal = Vector(-1, 0);
                    side = CollisionSide::Left;
                }
            } else {
                if (inverseEntry.y > 0) {
                    normal = Vector(0, -1);
                    side = CollisionSide::Bottom;
                } else {
                    normal = Vector(0, 1);
                    side = CollisionSide::Top;
                }
            }

            dist = entryTime;

            return SweptCollision(normal, dist, side);
        }

        void Collision::ResolveIndividualSweptCollision(Entity* resolve, SweptCollision collision, float remainingTime) {
            Components::Physics* rPhysics = resolve->GetComponent<Components::Physics>();
            float dt = ForLease->FrameRateController().GetDt();

            Vector velocity = rPhysics->Velocity * dt;

            ResolveIndividualSweptCollision(resolve, velocity, collision);

            //if (collision.Side != CollisionSide::None) {
            //    float dotprod = (rPhysics->Velocity.x * collision.Normal.y + rPhysics->Velocity.y * collision.Normal.x) * remainingTime;
            //    rPhysics->Velocity.x = dotprod * collision.Normal.y;
            //    rPhysics->Velocity.y = dotprod * collision.Normal.x;
            //}

            if (collision.SelfSide == CollisionSide::Top || collision.SelfSide == CollisionSide::Bottom) {
                rPhysics->Velocity.y = 0;
            } else if (collision.SelfSide == CollisionSide::Left || collision.SelfSide == CollisionSide::Right) {
                rPhysics->Velocity.x = 0;
            }
        }

        void Collision::ResolveIndividualSweptCollision(Entity* resolve, Vector movement, SweptCollision collision) {
            Components::Transform* rTransform = resolve->GetComponent<Components::Transform>();

            rTransform->Position += movement * (collision.Distance - Epsilon);
        }

        void Collision::CheckAndResolveCollision(Entity* entity, std::vector<Entity *>& entities) {
            Components::Transform* transform = entity->GetComponent<Components::Transform>();
            Components::Collision* collision = entity->GetComponent<Components::Collision>();
            Components::Physics* physics = entity->GetComponent<Components::Physics>();

            // Set up rays
            unsigned numVertsX = ceil(collision->ScaledWidth() / VertexSpacing) + 1;
            unsigned numVertsY = ceil(collision->ScaledHeight() / VertexSpacing) + 1;

            // Make sure we have at least the default # of verts per side (2, one for each corner)
            numVertsX = numVertsX < DefaultVertsPerSide ? DefaultVertsPerSide : numVertsX;
            numVertsY = numVertsY < DefaultVertsPerSide ? DefaultVertsPerSide : numVertsY;

            float xSpacing = collision->ScaledWidth() / (numVertsX - 1);
            float ySpacing = collision->ScaledHeight() / (numVertsY - 1);

            Point position = collision->Position();
            Vector halfDimensions = Vector(collision->ScaledHalfWidth(), collision->ScaledHalfHeight());
            Vector velocity = physics->GetFrameVelocity();

            CollisionSide side = CollisionSide::None;
            float dist = 9999;
            Entity* collidedWith = 0;

            for (unsigned i = 0; i < numVertsX; ++i) {
                Point point(position[0] - halfDimensions[0] + (i * xSpacing), position[1] - halfDimensions[1]);
                Ray ray1(point, velocity, velocity.Magnitude(), 1);
                Entity* collision1 = Ray::CheckCollisions(ray1, entities, entity);

                point[1] = position[1] + halfDimensions[1];
                Ray ray2(point, velocity, velocity.Magnitude(), 1);
                Entity* collision2 = Ray::CheckCollisions(ray2, entities, entity);

                if (collision1) {
                    if (ray1.GetLastDistance() < dist && ray1.GetLastDistance() > 0) {
                        dist = ray1.GetLastDistance();
                        side = ray1.GetLastSide();
                        collidedWith = collision1;
                    }
                }

                if (collision2) {
                    if (ray2.GetLastDistance() < dist && ray2.GetLastDistance() > 0) {
                        dist = ray2.GetLastDistance();
                        side = ray2.GetLastSide();
                        collidedWith = collision2;
                    }
                }
            }

            for (unsigned i = 0; i < numVertsY; ++i) {
                //Point toResolve(toResolvePosition[0] - toResolveHalfWidth + toResolveCollision->OffsetX, toResolvePosition[1] - toResolveHalfHeight + (i * ySpacing) + toResolveCollision->OffsetY);
                //CheckRay(toResolve, velocity, other, side, dist);
                //toResolve[0] = toResolvePosition[0] + toResolveHalfWidth + toResolveCollision->OffsetX;
                //CheckRay(toResolve, velocity, other, side, dist);

                Point point(position[0] - halfDimensions[0], position[1] - halfDimensions[1] + (i * ySpacing));
                Ray ray1(point, velocity, velocity.Magnitude(), 1);
                Entity* collision1 = Ray::CheckCollisions(ray1, entities, entity);

                point[0] = position[0] + halfDimensions[0];
                Ray ray2(point, velocity, velocity.Magnitude(), 1);
                Entity* collision2 = Ray::CheckCollisions(ray2, entities, entity);

                if (collision1) {
                    if (ray1.GetLastDistance() < dist && ray1.GetLastDistance() > 0) {
                        dist = ray1.GetLastDistance();
                        side = ray1.GetLastSide();
                        collidedWith = collision1;
                    }
                }

                if (collision2) {
                    if (ray2.GetLastDistance() < dist && ray2.GetLastDistance() > 0) {
                        dist = ray2.GetLastDistance();
                        side = ray2.GetLastSide();
                        collidedWith = collision2;
                    }
                }
            }

            collision->CollidedWithSide = side;

            if (side == CollisionSide::Bottom || side == CollisionSide::Top) {
                velocity[1] = 0.0f;
                physics->Velocity[1] = 0.0f;
            }
            else if (side == CollisionSide::Left || side == CollisionSide::Right) {
                velocity[0] = 0.0f;
                physics->Velocity[0] = 0.0f;
            }

            if (dist > 1) dist = 1;
            if (dist < 0) // This shit shouldn't happen
                return;

            transform->Position += velocity * dist;

            //Vector correction = (velocity * dist) - (velocity * Epsilon * 10);

            collision->CollidedLastFrame = true;
            //collision->CollidedWith = collidedWith;

            ForLease->Dispatcher.DispatchToParent(new CollisionEvent(collidedWith), entity);
            ForLease->Dispatcher.DispatchToParent(new CollisionEvent(entity), collidedWith);

            //transform->Position += correction;
        }

        /*!
            Checks collision between two entities.

            \param Entity1
                A pointer to the first Entity.

            \param Entity2
                A pointer to the second Entity.

            \return
                True if they are colliding, false if not.

            \bug
                Not well done.
        */
        bool Collision::CheckCollision(Entity* entity1, Entity* entity2) {
            Components::Transform* entity1Transform = entity1->GetComponent<Components::Transform>();
            Components::Transform* entity2Transform = entity2->GetComponent<Components::Transform>();
            Components::Collision* entity1Collision = entity1->GetComponent<Components::Collision>();
            Components::Collision* entity2Collision = entity2->GetComponent<Components::Collision>();
            Point entity1Position = entity1Collision->Position();
            Point entity2Position = entity2Collision->Position();

            //if (entity1Collision->CollidedLastFrame || entity2Collision->CollidedLastFrame) return false;

            //LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true);

            float ent1HalfWidth = entity1Collision->ScaledHalfWidth();
            float ent1HalfHeight = entity1Collision->ScaledHalfHeight();
            float ent2HalfWidth = entity2Collision->ScaledHalfWidth();
            float ent2HalfHeight = entity2Collision->ScaledHalfHeight();

            //renderer->DrawRectangle(entity1Position, entity1Collision->Width * entity1Transform->ScaleX, entity1Collision->Height * entity1Transform->ScaleY);

//            entity1Position[0] += entity1Collision->OffsetX;
//            entity1Position[1] += entity1Collision->OffsetY;
//            entity2Position[0] += entity2Collision->OffsetX;
//            entity2Position[1] += entity2Collision->OffsetY;

            bool collided =
                  !(entity2Position[0] - ent2HalfWidth > entity1Position[0] + ent1HalfWidth ||
                    entity2Position[0] + ent2HalfWidth < entity1Position[0] - ent1HalfWidth ||
                    entity2Position[1] + ent2HalfHeight < entity1Position[1] - ent1HalfHeight ||
                    entity2Position[1] - ent2HalfHeight > entity1Position[1] + ent1HalfHeight);

            if (!collided) return false;

            entity1Collision->CollidedLastFrame = true;
            entity2Collision->CollidedLastFrame = true;

            //entity1Collision->CollidedWith = entity2;
            //entity2Collision->CollidedWith = entity1;

            return true;

//            if (entity2Position[0] + entity2Collision->Width/2 > entity1Position[0] &&
//                entity1Position[0] + entity1Collision->Width/2 > entity2Position[0] &&
//                entity2Position[1] + entity2Collision->Height/2 > entity1Position[1] &&
//                entity1Position[1] + entity1Collision->Height/2 > entity2Position[1]) {
//                    entity1Collision->CollidedLastFrame = true;
//                    entity2Collision->CollidedLastFrame = true;
//                    std::cout << "Collision detected." << std::endl;
//                    return true;
//                }
//
//            return false;

//            return !(entity2Position[0] > (entity1Position[0] + entity1Collision->Width / 2) ||
//                    (entity2Position[0] + entity2Collision->Width / 2) < entity1Position[0] ||
//                     entity2Position[1] > (entity1Position[0] + entity1Collision->Height / 2) ||
//                    (entity2Position[1] + entity2Collision->Height / 2) < entity1Position[1]);

//            return (std::abs(entity1Position[0] - entity2Position[0]) * 2 < (entity1Collision->Width + entity2Collision->Width)) &&
//                   (std::abs(entity1Position[1] - entity2Position[1]) * 2 < (entity1Collision->Height + entity2Collision->Height));

            /*return !(entity2Position[0] - entity2Collision->Width / 2 > entity1Position[0] + entity1Collision->Width / 2 ||
                     entity2Position[0] + entity2Collision->Width / 2 < entity1Position[0] - entity1Collision->Width / 2 ||
                     entity2Position[1] + entity2Collision->Height / 2 < entity1Position[1] - entity1Collision->Height / 2 ||
                     entity2Position[1] - entity2Collision->Height / 2 > entity1Position[1] + entity1Collision->Height / 2);*/

        }

        /*!
            Resolves collisions between two objects.

            \param entity1
                The first entity to resolve collisions for.

            \param entity2
                The second entity to resolve collisions for.

            \bug
                Not well done.
        */
        void Collision::ResolveCollision(Entity* entity1, Entity* entity2) {
            Components::Collision* entity1Collision = entity1->GetComponent<Components::Collision>();
            Components::Collision* entity2Collision = entity2->GetComponent<Components::Collision>();

//            std::cout << entity1Collision->ResolveCollisions << " " << entity2Collision->ResolveCollisions << std::endl;

            if (!(entity1Collision->ResolveCollisions) || !(entity2Collision->ResolveCollisions)) return;

            //if (entity1->HasComponent(ComponentType::EnemyPace)) {
            //    entity2->GetComponent<Components::Transform>()->Position += entity1->GetComponent<Components::EnemyPace>()->LastMovement(true) * (1 + Epsilon);
            //    return;
            //}

//            std::cout << "Resolving collision." << std::endl;

            bool entity1HasPhysics = entity1->HasComponent(ComponentType::Physics);
            bool entity2HasPhysics = entity2->HasComponent(ComponentType::Physics);

            // Only one entity has physics--only going to resolve on one entity
            if (entity1HasPhysics != entity2HasPhysics) {
                if (entity1HasPhysics) {
                    ResolveCollisionOneEntityOnly(entity1, entity2);
                    return;
                } else {
                    ResolveCollisionOneEntityOnly(entity2, entity1);
                    return;
                }
            }

            // No physics on either entity--not going to resolve.
            if (entity1HasPhysics == false) return;

            //if (entity1Collision->IsPacingPlatform()) {
            //    ResolveCollisionOneEntityOnly(entity2, entity1);
            //}
            //else if (entity2Collision->IsPacingPlatform()) {
            //    ResolveCollisionOneEntityOnly(entity1, entity2);
            //}

            /*else {*/
                // Both entities have physics.  We'll try to resolve the collision on both.
                // We'll do it stupidly for now
                ResolveCollisionOneEntityOnly(entity1, entity2);
                ResolveCollisionOneEntityOnly(entity2, entity1);
            //}
        }



        /*!
            Resolves collisions on one entity.

            \param toResolve
                A pointer to the Entity to resolve collisions on.

            \param other
                A pointer that toResolve is colliding with.
        */
        void Collision::ResolveCollisionOneEntityOnly(Entity* toResolve, Entity* other) {
            ResolveCollisionBoundingBox(toResolve, other);
        }

        void Collision::ResolveCollisionBoundingBox(Entity* toResolve, Entity* other) {
            Components::Transform* toResolveTransform = toResolve->GetComponent<Components::Transform>(true);
            Components::Physics*   toResolvePhysics = toResolve->GetComponent<Components::Physics>(true);
            Components::Collision* toResolveCollision = toResolve->GetComponent<Components::Collision>(true);

            Components::Transform* otherTransform = other->GetComponent<Components::Transform>(true);
            Components::Collision* otherCollision = other->GetComponent<Components::Collision>(true);

            //if (toResolveCollision->IsPacingPlatform() && other->HasComponent(ComponentType::Physics)) {
            //    otherTransform->Position += toResolve->GetComponent<Components::EnemyPace>()->LastMovement(true);
            //    other->GetComponent<Components::Physics>()->Velocity += toResolvePhysics->Velocity;
            //    return;
            //}

            //if (otherCollision->IsPacingPlatform()) {
                //toResolveTransform->Position = toResolveTransform->Position + other->GetComponent<Components::EnemyPace>()->LastMovement(true);
                //toResolvePhysics->Acceleration = other->GetComponent<Components::EnemyPace>()->LastMovement(true) * (1/ForLease->FrameRateController().GetDt());

                //toResolvePhysics->Velocity += other->GetComponent<Components::Physics>()->Velocity;
                //std::cout << "Here" << std::endl;
                //toResolveCollision->VelocityModifier = other->GetComponent<Components::Physics>()->Velocity;
            //}

            Vector velocity = toResolvePhysics->Velocity * ForLease->FrameRateController().GetDt();
            toResolveTransform->Position -= velocity;
            Point toResolvePosition = toResolveTransform->Position;
            //toResolvePosition -= velocity;

            float toResolveHalfWidth = toResolveCollision->Width / 2 * toResolveTransform->ScaleX;
            float toResolveHalfHeight = toResolveCollision->Height / 2 * toResolveTransform->ScaleY;

            unsigned numVertsX = ceil(toResolveCollision->ScaledWidth() / otherCollision->ScaledWidth()) + 1;
            unsigned numVertsY = ceil(toResolveCollision->ScaledHeight() / otherCollision->ScaledHeight()) + 1;

            // Make sure we have at least the default # of verts per side (2, one for each corner)
            numVertsX = numVertsX < DefaultVertsPerSide ? DefaultVertsPerSide : numVertsX;
            numVertsY = numVertsY < DefaultVertsPerSide ? DefaultVertsPerSide : numVertsY;

            float xSpacing = toResolveCollision->ScaledWidth() / (numVertsX - 1);
            float ySpacing = toResolveCollision->ScaledHeight() / (numVertsY - 1);

            //LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true);

            CollisionSide side;
            float dist = 9999;

            //std::cout << "X: " << numVertsX << " | Y: " << numVertsY << std::endl;

            for (unsigned i = 0; i < numVertsX; ++i) {
                Point toResolve(toResolvePosition[0] - toResolveHalfWidth + (i * xSpacing) + toResolveCollision->OffsetX, toResolvePosition[1] - toResolveHalfHeight + toResolveCollision->OffsetY);
                CheckRay(toResolve, velocity, other, side, dist);
                toResolve[1] = toResolvePosition[1] + toResolveHalfHeight + toResolveCollision->OffsetY;
                CheckRay(toResolve, velocity, other, side, dist);
            }

            for (unsigned i = 0; i < numVertsY; ++i) {
                Point toResolve(toResolvePosition[0] - toResolveHalfWidth + toResolveCollision->OffsetX, toResolvePosition[1] - toResolveHalfHeight + (i * ySpacing) + toResolveCollision->OffsetY);
                CheckRay(toResolve, velocity, other, side, dist);
                toResolve[0] = toResolvePosition[0] + toResolveHalfWidth + toResolveCollision->OffsetX;
                CheckRay(toResolve, velocity, other, side, dist);
            }

            if (dist > 1) return; // This should never happen

            toResolveCollision->CollidedWithSide = side;

            //dist -= 1;
            //toResolveTransform->Position += velocity * dist;

            if (side == CollisionSide::Bottom || side == CollisionSide::Top) {
                velocity[1] = 0.0f;
                toResolvePhysics->Velocity[1] = 0.0f;
            }
            else {
                velocity[0] = 0.0f;
                toResolvePhysics->Velocity[0] = 0.0f;
            }

            toResolveTransform->Position += velocity;

            //dist = 1.0f - dist;
            //velocity = velocity * dist;
            //toResolveTransform->Position += velocity;
            //std::cout << velocity << std::endl;

            //std::cout << "In Here " << ++i << std::endl;

            //toResolveTransform->Position += velocity;

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //toResolveTransform->Position[1] -= toResolvePhysics->Velocity[1] * 2 * ForLease->FrameRateController().GetDt();
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //toResolvePhysics->Velocity[1] = 0;
        }

        void Collision::CheckRay(Point point, Vector velocity, Entity* other, CollisionSide& side, float& dist) {
            //ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->DrawRectangleFilled(point, 0.25, 0.25, 0);

            Ray toResolveRay(point, velocity, velocity.Magnitude(), 1);

            if (toResolveRay.IsColliding(other) && toResolveRay.GetLastDistance() < dist && toResolveRay.GetLastDistance() > Epsilon) {
                side = toResolveRay.GetLastSide();
                dist = toResolveRay.GetLastDistance();
            }
        }

        void Collision::ResolveCollisionMesh(Entity* toResolve, Entity* other) {

        }

        /*!
            Assumes bounding-box collisions.
        */
        Entity* Collision::GetEntityCollidingAtPoint(std::vector<Entity *>& entities, Point position) {
            Components::Transform* transform;
            Components::Collision* collision;
            for (Entity* entity : entities) {
                if (!entity->HasComponent(ComponentType::Transform)) continue;
                if (!entity->HasComponent(ComponentType::Collision)) continue;
                transform = entity->GetComponent<Components::Transform>();
                collision = entity->GetComponent<Components::Collision>();

                Point entPos = transform->Position;


                Point topLeft(entPos[0] - collision->Width / 2 * transform->ScaleX, entPos[1] + collision->Height / 2 * transform->ScaleY);
                Point topRight(entPos[0] + collision->Width / 2 * transform->ScaleX, entPos[1] + collision->Height / 2 * transform->ScaleY);
                Point botRight(entPos[0] + collision->Width / 2 * transform->ScaleX, entPos[1] - collision->Height / 2 * transform->ScaleY);
                Point botLeft(entPos[0] - collision->Width / 2 * transform->ScaleX, entPos[1] - collision->Height / 2 * transform->ScaleY);

                HalfPlane top(topLeft, topRight, entPos);
                HalfPlane right(topRight, botRight, entPos);
                HalfPlane left(topLeft, botLeft, entPos);
                HalfPlane bot(botLeft, botRight, entPos);

                if (right.Dot(position) < 0 && bot.Dot(position) < 0 && left.Dot(position) < 0 && top.Dot(position) < 0)
                    return entity;
            }

            return 0;
        }

        void Collision::PhysicsCompute(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent<Components::Physics>();
            Components::Transform* transformComponent = entity->GetComponent<Components::Transform>();

            float dt;

            if (physicsComponent->UnaffectedByTimeScaling) // This moves the same amount, no matter what time scaling is at
                dt = ForLease->FrameRateController().GetUnscaledDt();
            else                                           // This slows down as the world does
                dt = ForLease->FrameRateController().GetDt();

            physicsComponent->Velocity += physicsComponent->Acceleration * dt;
            //transformComponent->Position += physicsComponent->Velocity * dt;
        }

        void Collision::PhysicsCleanup(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent<Components::Physics>();
            physicsComponent->Acceleration = Vector(0, 0);
            physicsComponent->Force = Vector(0, 0);
        }

        void Collision::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Collision));
            Serializer collision = root.GetChild("Collision");
            collision.WriteUint("Type", static_cast<unsigned>(ComponentType::Collision));
            root.Append(collision, "Collision");
        }

        void Collision::Deserialize(Serializer& root) {
            Serializer collision = root.GetChild("Collision");
        }

    } // LevelComponents

} // ForLeaseEngine
