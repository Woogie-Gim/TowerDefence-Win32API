#pragma once

class Object;

// ���� ���� ��� Object(�÷��̾�, ����, �̻��� ��)�� �����ϴ� �̱��� �Ŵ���
class ObjectManager
{
public:
    DECLARE_SINGLE(ObjectManager);

    ~ObjectManager(); // �Ҹ���: ��ü Object ����

    // Object�� �Ŵ����� ���
    void Add(Object* object);
    // Object�� �Ŵ������� ���� (���ÿ� �޸� ����)
    void Remove(Object* object);
    // ��� Object ���� �� �޸� ����
    void Clear();
    // ������ ������ �� ���� delete 
    void CollectGarbage();


    // ���� ��ϵ� Object ��� ��ȯ
    const vector<Object*>& GetObjects() { return _objects; }

    // ���ø� �Լ�: ������ Ÿ���� Object�� ���� �� �ʱ�ȭ
    template<typename T>
    T* CreateObject()
    {
        // Ÿ�� üũ: T�� Object�� ��ӹ��� Ÿ������ ������ Ÿ�ӿ� �˻�
        static_assert(std::is_convertible_v<T*, Object*>);

        // ���� �Ҵ����� Object ����
        T* object = new T();

        // ��ü �ʱ�ȭ
        object->Init();

        // ����: ���⼭ Add()�� ȣ������ ������ �Ŵ����� ��ϵ��� ����
        // ���� �ǵ��� ���� CreateObject���� Add���� ���� ���ִ� �� ������ �� ����
        return object;
    }

private:
    // ���� �� �����ϴ� ��� Object �����͸� �����ϴ� �����̳�
    vector<Object*> _objects;
    // ���� ���� ť
    vector<Object*> _garbage;
};
