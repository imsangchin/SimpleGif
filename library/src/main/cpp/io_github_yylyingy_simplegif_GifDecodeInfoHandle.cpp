#include "io_github_yylyingy_simplegif_GifDecodeInfoHandle.h"
#include "GifDecoder.h"
#include <string.h>
#include <wchar.h>
#include <android/bitmap.h>

#ifdef __cplusplus
extern "C" {
#endif
//io.github.yylyingy.simplegif
JNIEXPORT jlong JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeInit
  (JNIEnv *env, jobject)
{
    return (jlong)new GifDecoder();
}

JNIEXPORT void JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeClose
  (JNIEnv *, jobject, jlong handle)
{
    delete (GifDecoder*)handle;
}

JNIEXPORT jboolean JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeLoad
  (JNIEnv * env, jobject, jlong handle, jstring fileName)
{
    const char* fileNameChars = env->GetStringUTFChars(fileName, 0);
    bool result = ((GifDecoder*)handle)->load(fileNameChars);
    env->ReleaseStringUTFChars(fileName, fileNameChars);
    return result;
}

JNIEXPORT jint JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeGetFrameCount
  (JNIEnv *, jobject, jlong handle)
{
    return ((GifDecoder*)handle)->getFrameCount();
}

JNIEXPORT jobject JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeGetFrame
  (JNIEnv *env, jobject, jlong handle, jint idx)
{
    GifDecoder* decoder = (GifDecoder*)handle;
    int imgWidth = decoder->getWidth();
    int imgHeight = decoder->getHeight();

    // Creaing Bitmap Config Class
    jclass bmpCfgCls = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID bmpClsValueOfMid = env->GetStaticMethodID(bmpCfgCls, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
    jobject jBmpCfg = env->CallStaticObjectMethod(bmpCfgCls, bmpClsValueOfMid, env->NewStringUTF("ARGB_8888"));

    // Creating a Bitmap Class
    jclass bmpCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapMid = env->GetStaticMethodID(bmpCls, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jobject jBmpObj = env->CallStaticObjectMethod(bmpCls, createBitmapMid, imgWidth, imgHeight, jBmpCfg);

    void* bitmapPixels;
    if (AndroidBitmap_lockPixels(env, jBmpObj, &bitmapPixels) < 0) {
        return 0;
    }
    //uint32_t* src = (uint32_t*) bitmapPixels;
    int stride = imgWidth * 4;
    int pixelsCount = stride * imgHeight;
    memcpy(bitmapPixels, decoder->getFrame(idx), pixelsCount);
    AndroidBitmap_unlockPixels(env, jBmpObj);

    return jBmpObj;
}

JNIEXPORT jobject JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_renderFrame
        (JNIEnv *env, jobject, jlong handle, jint idx,jobject jBmpObj){
    GifDecoder* decoder = (GifDecoder*)handle;
    int imgWidth = decoder->getWidth();
    int imgHeight = decoder->getHeight();
    void* bitmapPixels;
    if (AndroidBitmap_lockPixels(env, jBmpObj, &bitmapPixels) < 0) {
        return 0;
    }
    //uint32_t* src = (uint32_t*) bitmapPixels;
    int stride = imgWidth * 4;
    int pixelsCount = stride * imgHeight;
    memcpy(bitmapPixels, decoder->getFrame(idx), pixelsCount);
    AndroidBitmap_unlockPixels(env, jBmpObj);
    return jBmpObj;
}

JNIEXPORT jint JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeGetDelay
        (JNIEnv *, jobject, jlong handle, jint idx)
{
    return ((GifDecoder*)handle)->getDelay(idx);
}

JNIEXPORT jint JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeGetWidth
  (JNIEnv *, jobject, jlong handle)
{
    return ((GifDecoder*)handle)->getWidth();
}

JNIEXPORT jint JNICALL Java_io_github_yylyingy_simplegif_GifDecodeInfoHandle_nativeGetHeight
  (JNIEnv *, jobject, jlong handle)
{
    return ((GifDecoder*)handle)->getHeight();
}

#ifdef __cplusplus
}
#endif
