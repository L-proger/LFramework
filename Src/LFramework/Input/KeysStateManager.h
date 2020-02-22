#ifndef KeysStateManager_h__
#define KeysStateManager_h__

template<uint32_t Count, typename Owner>
class KeysStateManager {
public:
	friend Owner;
	KeysStateManager()
	{
		for (uint32_t i = 0; i < Count; ++i) {
			_key_states[i] = _key_pressed[i] = _key_released[i] = _key_states_previous[i] = false;
		}
	}
	void strobe();
private:
	bool _key_states[Count];
	bool _key_states_previous[Count];
	bool _key_pressed[Count];
	bool _key_released[Count];
};

template <uint32_t Count, typename Owner>
void KeysStateManager<Count, Owner>::strobe(){
	

	for (uint32_t i = 0; i < Count; ++i) {
		if (_key_states[i]) {  //if now pressed
			if (_key_states_previous[i]) {//if was pressed
				_key_pressed[i] = false;
			} else {
				_key_pressed[i] = true;
			}
		} else {
			_key_pressed[i] = false;
		}
		if (!_key_states[i]) {  //not pressed
			if (_key_states_previous[i]) {//if was pressed
				_key_released[i] = true;
			} else {
				_key_released[i] = false;
			}
		} else {
			_key_released[i] = false;
		}
		_key_states_previous[i] = _key_states[i];
	}
}


#endif // KeysStateManager_h__
