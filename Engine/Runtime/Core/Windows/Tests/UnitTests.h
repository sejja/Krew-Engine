#pragma once
#include "Framework.h"
using namespace testing;
#ifdef _EDITOR_
#ifndef _DEBUG

TEST("Test AudioEmitter Serialization",
    "Audio Components should be serialized properly") {
    Engine::Audio::AudioEmitter aud;
    aud.SetLoop(Random::RandomBool());
    aud.SetPitch(Random::RandomFloat(0, 1.f));
    aud.SetVolume(Random::RandomFloat(0, 1.f));

    float values[] = { aud.GetLoop(), aud.GetPitch(), aud.GetVolume() };

    nlohmann::json j;

    aud.ToJson(j);

    aud.FromJson(j);

    if (aud.GetLoop() != values[0]) { FAIL(); }
    if (aud.GetPitch() != values[1]) { FAIL(); }
    if (aud.GetVolume() != values[2]) { FAIL(); }
    SUCCEED();
}

TEST("Test 3D Audio Voice Count",
    "Each Audio Should use just one voice, and deallocate it on demmand") {
    auto mVoices = GAudio->GetTotalVoiceCount();

    ASSERT_THAT(GAudio->GetUsedVoiceCount() == 0);

    {
        Engine::GameObject smth;
        auto aud = smth.AddComponent<Engine::Audio::SoundEmitter3D>();
        aud->BeginPlay();
        ASSERT_THAT(GAudio->GetUsedVoiceCount() == 0);
        ASSERT_THAT(GAudio->GetTotalVoiceCount() == mVoices);
    }

    ASSERT_THAT(GAudio->GetUsedVoiceCount() == 0);

    SUCCEED();
}

TEST("Test 3D Audio Listener Identity",
    "Listeners should always be equal (if we have the same ammount of listeners)") {
    nlohmann::json json;

    char memory[sizeof(Engine::Audio::SoundListener3D)];
    
    {
        Engine::Audio::SoundListener3D listener;
        memcpy(&memory, &listener, sizeof(Engine::Audio::SoundListener3D));
        listener.ToJson(json);
    }

    {
        Engine::Audio::SoundListener3D listener;
        listener.FromJson(json);
        ASSERT_THAT(!memcmp(&memory, &listener, sizeof(Engine::Audio::SoundListener3D)));
    }

    SUCCEED();
}

float CompareVector2(Engine::Math::Vector3D *pSrc, Engine::Math::Vector3D* pDst)
{
    float d = 0.0f;

    d += fabs(pSrc->x - pDst->x);
    d += fabs(pSrc->y - pDst->y);
    d += fabs(pSrc->z - pDst->z);

    return d;
};

TEST("Vector Default Constructor",
    "Vectors should be initialized to 0") {

    Engine::Math::Vector3D v1; v1.x = v1.y = v1.z = 0.0f;
    Engine::Math::Vector3D temp;

    ASSERT_THAT(CompareVector2(&temp, &v1) < EPSILON);

    SUCCEED();
}

TEST("Vector Custom Constructor",
    "Vectors can contain custom values") {

    Engine::Math::Vector3D v1; v1.x = 4.0f; v1.y = 5.0f; v1.z = 9.0f;
    Engine::Math::Vector3D temp(4.0f, 5.0f, 9.0f);

    ASSERT_THAT(CompareVector2(&temp, &v1) < EPSILON);

    SUCCEED();
}

TEST("Audio Voices",
    "Voices should never left errant") {
    ASSERT_THAT((GAudio->GetUsedVoiceCount() + GAudio->GetFreeVoiceCount()) == GAudio->GetTotalVoiceCount());
    SUCCEED();
}

TEST("Scene Test",
    "Scenes should mantain ownership over objects") {
    GWorld->SetScene(new Engine::Scene);
    GWorld->GetScene()->SpawnObject();
    GWorld->GetScene()->SpawnObject();
    auto g = GWorld->GetScene()->SpawnObject("Getme");
    ASSERT_THAT(GWorld->GetScene()->GetObjectCount() == 3);
    ASSERT_THAT(GWorld->GetScene()->GetObjectByName("Getme") == g);
    GWorld->GetScene()->Clear();
    ASSERT_THAT(GWorld->GetScene()->GetObjectCount() == 0);
    SUCCEED();
}

TEST("World Test",
    "World should mantain ownership over scenes") {
    GWorld->SetScene(new Engine::Scene);
    Engine::Scene* scc = new Engine::Scene;
    scc->SpawnObject("ObjectFromScene2");
    GWorld->GetScene()->SpawnObject();
    GWorld->GetScene()->SpawnObject();
    auto g = GWorld->GetScene()->SpawnObject("Getme");
    ASSERT_THAT(GWorld->GetScene()->GetObjectCount() == 3);
    ASSERT_THAT(GWorld->GetScene()->GetObjectByName("Getme") == g);
    GWorld->GetScene()->Clear();
    ASSERT_THAT(GWorld->GetScene()->GetObjectCount() == 0);
    GWorld->SetScene(scc);
    ASSERT_THAT(GWorld->GetScene()->GetObjectCount() == 1);
    SUCCEED();
}

TEST("ForEach Test",
    "Should iterate through a container correctly") {
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    Engine::Array<int> i(a, 10);
    int sum = 0;
    Engine::FForEach(i.begin(), i.end(), [&sum](int i) {sum += i; });
    ASSERT_THAT(sum == 45);
    SUCCEED();
}

Engine::Array<int> times7(Engine::Array<int> const& numbers) {
    Engine::Array<int> results;
    for (auto& x : numbers) {
        results.push_back(x * 7);
    }
    return results;
}
TEST("Array Pushing", "Compare two arrays and compare results") {

    Engine::Array<int> inputs = Engine::Array<int>{ 3, 4, 7 };

    auto const results = times7(inputs);
    Engine::Array<int> expected = { 21, 28, 49 };

    for (int i = 0; i < 3; i++) if (results[i] != expected[i]) { FAIL(); }
    SUCCEED();
}

#endif
#endif