//
//	Particle.h
//	Good Neighbours
//
//	Created by Jon Ibarra on 06/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _PARTICLE__H_
#define _PARTICLE__H_

namespace Engine {
	namespace Graphics {
		class Particle {
		public:

			Particle(): mPosition(0.f), mVelocity(0.f), mColor(0.f), mLife(0.f), mCameraDistance(-1.f) {}
			//Particle(const size_t width, const size_t height,
			//	unsigned char* pixels = nullptr);
			~Particle() {}

			bool operator<(Particle& that)
			{
				// Sort in reverse order : far particles drawn first.
				return this->mLife > that.mLife;
			}

			glm::vec3		mPosition, mVelocity;
			glm::vec4		mColor;
			float			mSize, mAngle, mWeight;
			float			mLife;

			float			mCameraDistance;

		private:
		};
	}
}

#endif
