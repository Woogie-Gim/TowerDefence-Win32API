#pragma once

// Scene을 관리하는 매니저 클래스
class SceneManager
{
	DECLARE_SINGLE(SceneManager); // 싱글톤 매크로

public:
	// Scene 매니저 초기화
	void Init();
	// 현재 Scene 업데이트 호출
	void Update();
	// 현재 Scene 렌더링 호출
	void Render(HDC hdc);
	// 현재 Scene 삭제
	void Clear();

public:
	// Scene 변경
	void ChangeScene(SceneType sceneType);

private:
	class Scene* _scene;             // 현재 활성화된 Scene
	SceneType _sceneType = SceneType::None;  // 현재 Scene 타입
};
