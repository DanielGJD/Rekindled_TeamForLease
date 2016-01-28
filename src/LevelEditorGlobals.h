namespace ForLeaseEngine
{
     namespace LevelEditorGlobals
    {
        extern SDL_Window* window;
        extern Point       mousePos;

        extern LevelComponents::Renderer* render;
        extern LevelComponents::Physics*  levelPhysics;
        extern LevelComponents::Light*    levelLight;
        extern Vector gravity;

        extern Entity*                            selection;
        extern Components::Transform*             selTran;
        extern Components::Model*                 selModel;
        extern Components::Camera*                selCamera;
        extern Components::Light*                 selLight;
        extern Components::Collision*             selCollision;
        extern Components::Physics*               selPhysics;
        //extern Components::SoundEmitter*          selSound;
        extern Components::Sprite*                selSprite;
        extern Components::SpriteText*            selSprtxt;
        extern Components::CharacterController*   selController;
        extern Components::VisionCone*            selVision;
        extern Components::Menu*                  selMenu;
        extern Components::DragWithMouse*         selDrag;
        extern Components::ScaleWithKeyboard*     selScale;
        extern Components::TransformModeControls* selTMC;
        extern Components::EnemyAI*               selEnemyAI;
        extern Components::FadeWithDistance*      selFade;
        extern Components::ChangeLevelOnCollide*  selChange;
        extern Components::BackgroundMusic*       selMusic;
        extern Components::ParticleColorAnimator* selPartColor;
        extern Components::ParticleEmitter*       selPartEmitter;
        extern Components::ParticleSystem*        selPartSystem;


        extern Entity*                camera;
        extern Components::Transform* camTrans;
        extern Components::Camera*    camCamera;

        extern std::vector<std::string> meshNames;
        extern std::vector<std::string> componentNames;
        //extern std::vector<std::string> soundNames;
        extern std::vector<std::string> archetypeNames;
        extern std::vector<std::string> fontNames;
        extern std::vector<std::string> animationNames;
        extern std::vector<std::string> textureNames;

        extern std::map<std::string, ComponentType> reqMap;
        extern Serializer copyEntity;

        extern bool clickAdd;
        extern bool newSelect;
        extern bool moveMode;
        extern bool showWindow;
        extern bool selMade;
        extern bool archSpawn;
        extern bool selMode;
        extern bool toSave;
        extern bool toLoad;
        extern bool delobj;
        extern bool copySet;
        extern bool setLiked;
        extern bool setHated;
        extern bool setTarget;
        extern bool walkSound;
        extern bool jumpSound;
        extern bool landSound;
        extern bool walkAni;
        extern bool jumpAni;
        extern bool setName;

        extern char entName[70];
        extern char spriteTextBuf[500];
        extern char statefile[70];
        extern char statename[70];
        extern char meshfile[70];
        extern char archetypefile[70];
        extern char soundfile[70];
        extern char fontfile[70];
        extern char enemyHateN[70];
        extern char enemyLikeN[70];
        extern char enemyHateS[70];
        extern char enemyLikeS[70];
        extern char animationfile[70];
        extern char spriteSource[70];
        extern char changeLevel[70];
        extern char changeObject[70];

        extern float spriteTextColor[4];
        extern float visionConeColor[4];
        extern float happyColor[4];
        extern float detectionColor[4];
        extern float noDetectionColor[4];
        extern float spriteColor[4];
        extern float meshColor[4];
        extern float lightColor[4];

        extern const char* archToSpawn;
        extern int lightRays;
    }
}
