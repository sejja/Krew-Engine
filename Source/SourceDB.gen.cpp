/*This file has been automatically generated, 
DO NOT MODIFY UNDER ANY CIRCUMSTANCE, 
//
Good Neighbours Header Tool - 2021*/

#include<Shared.h>
template<typename T>  class TCreator : public Engine::Factory::ICreator {Engine::IBase* Create() override {return new T();}};
int DllMain() {
	static bool loaded = false;
	if(!loaded) {
	loaded = true;}


	return 1;

}
