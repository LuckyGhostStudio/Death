#include <Lucky.h>

class LuckyEditor : public Lucky::Application
{
public:
	LuckyEditor()
	{

	}

	~LuckyEditor()
	{

	}
};

Lucky::Application* Lucky::CreateApplication()
{
	return new LuckyEditor();
}