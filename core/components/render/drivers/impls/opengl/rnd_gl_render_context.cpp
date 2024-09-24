#include "rnd_gl_render_context.h"
#include <common.h>
#include "rnd_gl_driver.h"
#include <engine_log.h>

rnd::driver::gl::render_context::render_context(GLADloadproc load)
	: loader(load) 
{
	const bool success = gladLoadGLLoader(load);
	ASSERT_MSG(success, "glad didnt loaded in this process!");

	egLOG("render_context/init",
		"OpenGL Info: \n"
		"Vendor: {0}  \n"
		"Renderer: {1}\n"
		"Version: {2}\n",
		(const char*)glGetString(GL_VENDOR),
		(const char*)glGetString(GL_RENDERER),
		(const char*)glGetString(GL_VERSION));

	ASSERT_MSG(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Engine requires at least OpenGL version 4.5!");
}

std::unique_ptr<rnd::driver::driver_interface> rnd::driver::gl::render_context::create_driver()
{
	return std::make_unique<gl::driver>();
}
