#include <jni.h>
#include <string>

extern int main(int argc, char **argv);

extern "C" JNIEXPORT jstring JNICALL
Java_com_epai_makesdna_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    //  生成DNA文件
    char *par1="makesdna";
    char *par2="/data/data/com.epai.makesdna/dna.c";
    char *par3="/data/data/com.epai.makesdna/dna_type_offsets.h";
    char *par4="/data/data/com.epai.makesdna/dna_verify.c";
    char*par5= "/data/data/com.epai.makesdna/makesdna/";
    char* pars[5]={par1,par2,par3,par4,par5};
    main(5,pars);

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}