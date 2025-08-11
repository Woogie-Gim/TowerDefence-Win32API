#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"

ObjectManager::~ObjectManager()
{
    // 매니저 소멸 시, 남아 있는 모든 Object 정리
    Clear();
}

void ObjectManager::Add(Object* object)
{
    if (object == nullptr)
        return; // 잘못된 포인터 방지

    // 이미 등록된 객체인지 확인
    auto findIt = std::find(_objects.begin(), _objects.end(), object);

    if (findIt != _objects.end())
        return; // 중복 방지

    // 새 Object 등록
    _objects.push_back(object);
}

void ObjectManager::Remove(Object* object)
{
    if (!object) return;

    // 1) _objects 에서 '그 포인터' 딱 하나만 제거
    auto it = std::find(_objects.begin(), _objects.end(), object);
    if (it != _objects.end())
        _objects.erase(it);

    // 2) _garbage 중복 삽입 방지
    auto git = std::find(_garbage.begin(), _garbage.end(), object);
    if (git == _garbage.end())
        _garbage.push_back(object);
}

void ObjectManager::CollectGarbage()
{
    for (Object* obj : _garbage)
        delete obj;
    _garbage.clear();
}

void ObjectManager::Clear()
{
    // 모든 Object를 순회하며 메모리 해제
    std::for_each(_objects.begin(), _objects.end(), [=](Object* obj) { delete obj; });

    // 컨테이너 비우기
    _objects.clear();
}