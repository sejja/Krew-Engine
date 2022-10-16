//
//	RigidBody.h
//	Good Neighbours
//
//	Created by Unai Morentin 2021/09/24
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _RGBdy__H_
#define _RGBdy__H_


namespace Engine {
	struct ENGINE_API RigidBody : public Component{
		RigidBody();
		void Tick() override;
		void OnEditor() override;

		void FromJson(const nlohmann::json& val);
		void ToJson(nlohmann::json& val) const;

		glm::vec3 m_vel; // velocity
		glm::vec3 m_acc;  //acceleration
		float m_drag;
		glm::vec3 m_prev_pos;
		glm::vec3 m_new_pos;
		float m_inv_mass;
		bool m_add_gravity;

	private:
		void integrate(float t);
		void add_force(glm::vec3 _force);
	};

}


#endif