#include "Shared.h"

namespace Engine {
	RigidBody::RigidBody() : m_inv_mass(1.f), m_drag(1.f), m_vel(0.f, 0.f, 0.f), m_acc(0.f, 0.f, 0.f), m_add_gravity(true) {

	}

	void RigidBody::integrate(float t) {
		glm::vec3 computed_acc = m_acc * m_inv_mass;
		m_vel += computed_acc * t; 
		m_vel *= m_drag;
		m_new_pos = m_prev_pos + m_vel * t; 
		m_acc = glm::vec3(0, 0, 0); //reset acceleration
	}

	void RigidBody::Tick() {
		if (m_inv_mass && m_add_gravity)
			add_force(GPhysics->Instance().mGravity / m_inv_mass);

		m_prev_pos = GetOwner()->GetPosition();
		// Integrate physics
		integrate(GFrames->GetFrameTime());
		GetOwner()->SetPosition(m_new_pos);
	}
	
	void RigidBody::OnEditor()
	{
#ifdef _EDITOR_
		ImGui::DragFloat("Inverse Mass", &m_inv_mass, 1.f, 0.f);
		ImGui::InputFloat3("Vel", &m_vel[0]);
		ImGui::Checkbox("Add Gravity", &m_add_gravity);
#endif
	}

	void RigidBody::FromJson(const nlohmann::json& val)
	{
		fetch(m_inv_mass, val, "InverseMass");
		fetch(m_vel.x, val, "Velx");
		fetch(m_vel.y, val, "Vely");
		fetch(m_vel.z, val, "Velz");
		fetch(m_add_gravity, val, "AddGravity");
	}
	
	void RigidBody::ToJson(nlohmann::json& val) const
	{
		val["InverseMass"] = m_inv_mass;
		val["AddGravity"] = m_add_gravity;
		val["Velx"] = m_vel.x;
		val["Vely"] = m_vel.y;
		val["Velz"] = m_vel.z;
		val["AddGravity"] = m_add_gravity;
	}

	void RigidBody::add_force(glm::vec3 _force) {
		m_acc += _force;
	}
}