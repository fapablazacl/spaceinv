#pragma once

#include "xe/sg/Pipeline.hpp"
#include "gl3/Device.hpp"
#include "gl3/Program.hpp"

#include "Mesh.hpp"

class PhongPipeline : public xe::sg::Pipeline {
public:
	explicit PhongPipeline(gl3::Device *device);

	~PhongPipeline();

	virtual void beginFrame(const xe::Vector4f &color) override;
	virtual void endFrame() override;

	virtual void render(xe::sg::Light *light) override;
	virtual void render(xe::sg::Camera *camera) override;
	virtual void render(xe::sg::Geometry *geometry) override;
	
	void render(Mesh *mesh);

	virtual void setWorldTransform(const xe::Matrix4f &world) override;

protected:
	xe::Matrix4f getPVW() const;

	void renderMaterial(Material &material);

	void renderMesh(Mesh &mesh);

	void updateMvp();

private:
	gl3::Device *m_device = nullptr;
	gl3::ProgramPtr m_program;

	xe::Matrix4f m_proj;
    xe::Matrix4f m_view;
    xe::Matrix4f m_world;
};

typedef std::unique_ptr<PhongPipeline> PhongPipelinePtr;