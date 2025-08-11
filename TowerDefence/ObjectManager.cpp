#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"

ObjectManager::~ObjectManager()
{
    // �Ŵ��� �Ҹ� ��, ���� �ִ� ��� Object ����
    Clear();
}

void ObjectManager::Add(Object* object)
{
    if (object == nullptr)
        return; // �߸��� ������ ����

    // �̹� ��ϵ� ��ü���� Ȯ��
    auto findIt = std::find(_objects.begin(), _objects.end(), object);

    if (findIt != _objects.end())
        return; // �ߺ� ����

    // �� Object ���
    _objects.push_back(object);
}

void ObjectManager::Remove(Object* object)
{
    if (!object) return;

    // 1) _objects ���� '�� ������' �� �ϳ��� ����
    auto it = std::find(_objects.begin(), _objects.end(), object);
    if (it != _objects.end())
        _objects.erase(it);

    // 2) _garbage �ߺ� ���� ����
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
    // ��� Object�� ��ȸ�ϸ� �޸� ����
    std::for_each(_objects.begin(), _objects.end(), [=](Object* obj) { delete obj; });

    // �����̳� ����
    _objects.clear();
}