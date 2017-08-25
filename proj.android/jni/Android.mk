LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/GLES-Render/B2DebugDrawLayer.cpp \
                   ../../Classes/GLES-Render/GB2ShapeCacheX.cpp \
                   ../../Classes/GLES-Render/GLESRender.cpp \
                   ../../Classes/GLES-Render/VisibleRect.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Airhockey.cpp \
									 ../../Classes/Ball.cpp \
									 ../../Classes/GameButton.cpp \
                   ../../Classes/GameList.cpp \
                   ../../Classes/GameScene.cpp \
									 ../../Classes/Holdrace.cpp \
									 ../../Classes/MainMenuScene.cpp \
					../../Classes/SharedBall.cpp \
					../../Classes/SmashBall.cpp \
                   ../../Classes/Pinball.cpp \
									 ../../Classes/PlayerMenuScene.cpp \
                   ../../Classes/Shared.cpp \
                   ../../Classes/SoundManager.cpp \
									 ../../Classes/SplashScene.cpp \
                   ../../Classes/Taprace.cpp\
                   					 ../../Classes/WetKitten.cpp \
                   					 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)



# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
