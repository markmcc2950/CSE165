#ifndef sound_h
#define sound_h

class sound {
	const char* filename;
	bool loop = false;
public:
	sound(const char*);

	void play();

	void stop();

	void playOnce();

	~sound();
};

#endif
