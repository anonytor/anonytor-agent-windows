#include "task.h"
class get_file_content:public task
{private:
	 string taskId;
	 string path;
public:
	get_file_content(string, string);
	void execute();
};

