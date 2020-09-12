#include "task.h"
class get_screenshot:public task
{private:
	 string taskId;
public:
	get_screenshot(string);
	void execute();
};

