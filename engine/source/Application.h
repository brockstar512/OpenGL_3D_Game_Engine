#pragma once

namespace engine{

	class Application
	{
		public:
			virtual bool Init() = 0;
			virtual void Update(float deltaTime) = 0;
			virtual void Destroy() = 0;

			void SetNeedsToBeClosed(bool value);
			bool NeedsToBeClosed() const;
	private:
		bool m_needsToBeClosed = false;
	};
}