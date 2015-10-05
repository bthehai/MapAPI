#include "stdafx.h"
#include "EventDispatcher.h"
#include "EventListener.h"
#include "Event.h"
#include "..\utils\pool\Inlist.h"


void EventDispatcher::bind( EventListener *pListener )
{
	if (LList<EventListener*>::find(m_pListeners, pListener) != NULL) {
		return;
	}
	m_pListeners = LList<EventListener*>::push(m_pListeners, new LList<EventListener*>(pListener));
}

void EventDispatcher::unbind( EventListener *pListener )
{
	m_pListeners = LList<EventListener*>::remove(m_pListeners, pListener);
}

void EventDispatcher::fire( Event *pEvent, void* data )
{
	//(LList<EventListener*>*)
	for (Inlist *pIt = (Inlist *)m_pListeners; pIt != NULL; pIt = pIt->next) {
		LList<EventListener*> *pTemp = (LList<EventListener*> *)pIt;
		tell(pTemp->data, pEvent, data);
	}
}
