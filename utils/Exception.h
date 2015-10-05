#pragma once

class Exception {
public:
	Exception(char* error) {
		m_Msg = error;
	}

	virtual ~Exception() {

	}

public:
	char* getErrorMessage() {
		return m_Msg;
	}

protected:
	char* m_Msg;

};