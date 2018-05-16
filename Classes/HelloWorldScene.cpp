#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "reader/CreatorReader.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	//creator::CreatorReader* reader = creator::CreatorReader::createWithFilename("creator/scenes/video/VideoPlayer.ccreator");

	//// will create the needed spritesheets + design resolution
	//reader->setup();

	//// get the scene graph
	//Scene* scene = reader->getSceneGraph();

	//// ...and use it
	////Director::getInstance()->replaceScene(scene);

	//return scene;
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
	
	//OGR
	auto sprite = Sprite3D::create("models/orc.c3b");
	sprite->setScale(5.f);
	sprite->setPosition(Vec2(300, 200));
	
	auto axe = Sprite3D::create("models/axe.c3b");
	sprite->getAttachNode("Bip001 R Hand")->addChild(axe);

	//our sprite object runs the action
	auto rotation = RotateBy::create(5, Vec3(0, 360, 0));
	sprite->runAction(RepeatForever::create(rotation));



	// the animation is contained in the .c3b file
	auto animation = Animation3D::create("models/orc.c3b");
	// creates the Action with Animation object
	auto animate = Animate3D::create(animation);
	// runs the animation
	sprite->runAction(RepeatForever::create(animate));


	addChild(sprite, 1); //adds sprite to scene, z-index: 1
	//

	//Girl
	auto girl = Sprite3D::create("models/ReskinGirl.c3b");
	girl->setScale(5.0f);
	girl->setPosition(Vec2(200, 100));
	girl->runAction(RepeatForever::create(rotation));

	// display the first coat
	auto girlTop0 = girl->getMeshByName("Girl_UpperBody01");
	girlTop0->setVisible(true);

	auto girlTop1 = girl->getMeshByName("Girl_UpperBody02");
	girlTop1->setVisible(false);

	// swap to the second coat
	girlTop0->setVisible(false);
	girlTop1->setVisible(true);

	addChild(girl, 2);





	//Camera
	auto s = Director::getInstance()->getWinSize();
	auto camera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);

	// set parameters for camera
	camera->setPosition3D(Vec3(0, 100, 100));
	camera->lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));

	addChild(camera); //add camera to the scene

	//CubeMap
	createSkybox();

	//lights(the are 4 types: ambiental, directional, spot and poit)
	//auto sun = AmbientLight::create(Color3B::BLACK);
	//addChild(sun);

	//auto light = DirectionLight::create(Vec3(-1.0f, -1.0f, 0.0f), Color3B::RED);
	//addChild(light);

	//auto lamp = PointLight::create(Vec3(0.0f, 0.0f, 0.0f), Color3B::BLUE, 10000.0f);
	//addChild(lamp);

	auto flashCone = SpotLight::create(Vec3(-1.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color3B::RED, 0.0, 0.5, 10000.0f);
	addChild(flashCone);




	CreateTerrain();


    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
    return true;
}

void HelloWorld::createSkybox()
{
	// create a textureCube object with six texture assets
	auto textureCube = TextureCube::create("models/skybox/left.jpg", "models/skybox/right.jpg", "models/skybox/top.jpg", "models/skybox/bottom.jpg", "models/skybox/front.jpg", "models/skybox/back.jpg");

	// set cube map texture parameters
	Texture2D::TexParams tRepeatParams;
	tRepeatParams.magFilter = GL_NEAREST;
	tRepeatParams.minFilter = GL_NEAREST;
	tRepeatParams.wrapS = GL_MIRRORED_REPEAT;
	tRepeatParams.wrapT = GL_MIRRORED_REPEAT;
	textureCube->setTexParameters(tRepeatParams);

	// create and set our custom shader
	auto shader = GLProgram::createWithFilenames("models/cube_map.vert", "models/cube_map.frag");
	auto state = GLProgramState::create(shader);

	// bind cube map texture to uniform
	state->setUniformTexture("u_cubeTex", textureCube);



	//Skybox
	// create a Skybox object
	auto box = Skybox::create();

	// set textureCube for Skybox
	box->setTexture(textureCube);

	// attached to scene
	addChild(box);
}

void HelloWorld::CreateTerrain() 
{
	Terrain::DetailMap r("terrain/dirt.jpg");
	Terrain::DetailMap g("terrain/Grass1.jpg");
	Terrain::DetailMap b("terrain/road.jpg");
	Terrain::DetailMap a("terrain/GreenSkin.jpg");

	Terrain::TerrainData data("terrain/heightmap16.jpg", "terrain/alphamap.png", r, g, b, a);
	auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
	addChild(terrain);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
