#pragma once

// �̱��� ���� ��ũ�� �����
// Ŭ���� ���ο� �����ؼ� �ش� Ŭ������ ������ �ν��Ͻ��� ���� �� ��ȯ��
// �����ڸ� private���� ���� �ܺο��� new ���� ���ϰ� ����
#define DECLARE_SINGLE(classname)           \
private:                                    \
    classname() {}                          /* �����ڸ� private���� ���� (�ܺ� ���� ����) */ \
public:                                     \
    static classname* GetInstance()         /* ���� ���� ������ ���� �ν��Ͻ� ��ȯ �Լ� */ \
    {                                       \
        static classname s_instance;        /* ���α׷� ��ü���� �ϳ��� ������ (���� ���� ����) */ \
        return &s_instance;                 \
    }

// �̱��� ��ü ���ٿ� ��ũ��
#define GET_SINGLE(classname)   classname::GetInstance()

// ������ ������ ���� ��ũ��
// delete �� nullptr�� �ʱ�ȭ�Ͽ� ���� ���� ����
#define SAFE_DELETE(ptr)        \
    if (ptr)                    \
    {                           \
        delete ptr;             \
        ptr = nullptr;          \
    }
