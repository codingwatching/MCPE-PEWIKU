#ifndef SoundSystemWin32_H__
#define SoundSystemWin32_H__

#include "SoundSystem.h"
#include <vector>
#include <string>

class SoundSystemWin32 : public SoundSystem
{
public:
    SoundSystemWin32();
    virtual ~SoundSystemWin32();

    virtual bool isAvailable() { return true; }
    virtual void setListenerPos(float x, float y, float z);
    virtual void setListenerAngle(float deg);
    virtual void playAt(const SoundDesc& desc, float x, float y, float z, float volume, float pitch);
    bool playMusicWavFile(const std::string& path, float volume);
    void stopMusic();
    void setMusicVolume(float volume);
    bool isMusicPlaying() const;

private:
#ifdef _WIN32
    struct ActiveVoice {
        void* buffer;
    };

    std::vector<ActiveVoice> _voices;
    void* _device;
    void* _musicBuffer;
    float _listenerX;
    float _listenerY;
    float _listenerZ;
    float _listenerDeg;
    void initDevice();
    void cleanupStoppedVoices();
    void cleanupAllVoices();
    static long toDSVolume(float gain);
    static long toDSPan(float pan);
    static const int MAX_VOICES = 16;
#endif
};

#endif /*SoundSystemWin32_H__*/
