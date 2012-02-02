SPARK Build guide
-----------------

Note:

	${SPARK_DIR} refers to the folder where SPARK is installed.


Source directories:

	- For the engine : ${SPARK_DIR}/projects/engine
	- For the demos : ${SPARK_DIR}/projects/demos


Recommended build directory:

	- For the engine : ${SPARK_DIR}/projects/build/engine
	- For the demos : ${SPARK_DIR}/projects/build/demos


To build a project (engine or demos):

	First, you have to know how CMake works. If not, there are plenty of tutorials on the net.
	When configuring projects for the first time, verify the variables which start with 'SPARK_',
	of 'DEMOS_' if you configure the demos.
	Note that SPARK release dlls are automatically copied to the 'demos/bin' folder.


Note:

	Libraries are put in the folder ${SPARK_DIR}/lib/<system-name>@<generator>/<build-type>
	where:
		<system-name> is the name of your OS (ex: Windows)
		<generator> is the name of the generator used (ex: Visual Studio 10)
		<build-type> is 'dynamic' or 'static', depending on the project settings. (see SPARK_STATIC_BUILD variable)

	Demos are put in ${SPARK_DIR}/demos/bin