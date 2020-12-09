#include "Entities.h"

Entities::Entities()
{

}

Entities::~Entities()
{
	if (m_path != nullptr)
	{
		delete m_path;
	}
}