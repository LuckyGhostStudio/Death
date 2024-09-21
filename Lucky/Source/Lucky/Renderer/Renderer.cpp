#include "lcpch.h"
#include "Renderer.h"

namespace Lucky
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();		// 场景数据

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();	// 设置场景 vp 矩阵
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);	// 设置 vp 矩阵的 Uniform 变量

		vertexArray->Bind();						// 绑定顶点数组
		RenderCommand::DrawIndexed(vertexArray);	// 绘制索引缓冲区
	}
}