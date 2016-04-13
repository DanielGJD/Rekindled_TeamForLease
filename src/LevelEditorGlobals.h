namespace ForLeaseEngine
{
     namespace LevelEditorGlobals
    {
        extern SDL_Window* window;
        extern Point       mousePos;

        extern LevelComponents::Renderer*     render;
        extern LevelComponents::Physics*      levelPhysics;
        extern LevelComponents::Light*        levelLight;
        extern LevelComponents::Checkpoint*   levelCheckpoint;
        extern LevelComponents::UsefulObject* levelUseful;
        extern Vector gravity;

        extern Entity*                             selection;
        extern Components::Transform*              selTran;
        extern Components::Model*                  selModel;
        extern Components::Camera*                 selCamera;
        extern Components::Light*                  selLight;
        extern Components::Collision*              selCollision;
        extern Components::Physics*                selPhysics;
        extern Components::SoundEmitter*           selSound;
        extern Components::Sprite*                 selSprite;
        extern Components::SpriteText*             selSprtxt;
        extern Components::CharacterController*    selController;
        extern Components::VisionCone*             selVision;
        extern Components::Menu*                   selMenu;
        extern Components::DragWithMouse*          selDrag;
        extern Components::ScaleWithKeyboard*      selScale;
        extern Components::TransformModeControls*  selTMC;
        extern Components::EnemyAI*                selEnemyAI;
        extern Components::FadeWithDistance*       selFade;
        extern Components::ChangeLevelOnCollide*   selChange;
        extern Components::BackgroundMusic*        selMusic;
        extern Components::ParticleColorAnimator*  selPartColor;
        extern Components::ParticleEmitter*        selPartEmitter;
        extern Components::ParticleSystem*         selPartSystem;
        extern Components::SimpleParticleDynamics* selPartDynamics;
        extern Components::Parallax*               selParallax;
        extern Components::Occluder*               selOccluder;
        extern Components::Checkpoint*             selCheckpoint;
        extern Components::Follow*                 selFollow;
        extern Components::EnemyPace*              selPace;
        extern Components::Health*                 selHealth;
        extern Components::OwlAI*                  selOwl;
        extern Components::MovingPlatform*         selMoving;
        extern Components::UsefulObject*           selUseful;
        extern Components::DamageOnCollide*        selDamage;
        extern Components::UsefulObjectInventory*  selInventory;
        extern Components::FinaleTwo*              selFinale2;
        extern Components::FinaleOne*              selFinale1;
        extern Components::Autoplay*               selAutoplay;

        extern Entity*                levelCamera;
        extern Entity*                camera;
        extern Components::Transform* camTrans;
        extern Components::Camera*    camCamera;

        extern std::vector<std::string> meshNames;
        extern std::vector<std::string> componentNames;
        extern std::vector<std::string> soundNames;
        extern std::vector<std::string> archetypeNames;
        extern std::vector<std::string> fontNames;
        extern std::vector<std::string> animationNames;
        extern std::vector<std::string> textureNames;
        extern std::vector<std::string> keyCodes;

        extern std::unordered_map<std::string, ComponentType> reqMap;
        extern Serializer copyEntity;

        extern bool clickAdd;
        extern bool moveMode;
        extern bool selMade;
        extern bool selMode;
        extern bool delobj;
        extern bool copySet;
        extern bool setFade;
        extern bool levelSaved;
        extern bool setFollow;

        extern char entName[128];
        extern char spriteTextBuf[512];
        extern char statefile[128];
        extern char statename[128];
        extern char archetypefile[128];
        extern char changeLevel[128];
        extern char changeObject[128];
        extern char enemyHateName[128];
        extern char enemyLikeName[128];
        extern char usefulFollowName[128];

        extern const char* archToSpawn;
        extern int eCount;
        extern int maxParticles;
        extern int particleBlend;
        extern int modelBlend;
        extern int lightBlend;
        extern int usefulCategory;
        extern float lightAngle;
        extern float visionAngle;
        extern float owlDir1;
        extern float owlDir2;
        extern float overlayColor[4];

        extern std::string levelDir;
        extern std::string blueprintDir;
    }
}
