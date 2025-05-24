#include <jni.h>
#include <string>
extern int main(int argc, char **argv);


extern "C" JNIEXPORT jstring JNICALL
Java_com_epai_makesrna_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    char *par1="/data/data/com.epai.makesrna/";
    char *par2="/data/data/com.epai.makesrna/";
    char *par3="/data/data/com.epai.makesrna/";
    char*pars[3]={par1,par2,par3};
    main(3,pars);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}