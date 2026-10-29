const PHASE_LABELS = [
	'a', 'b', 'd', 'c', 'none'
];
const PHASE_A = 0; // 00
const PHASE_B = 1; // 10
const PHASE_C = 3; // 11
const PHASE_D = 2; // 01
const PHASE_NONE = 4;

const channel_a_pin = io.pin(10, false);
const channel_b_pin = io.pin(11, false);

let phase_last = PHASE_NONE;
let counter = 0;

mainloop: while (true) {
	const channel_a = channel_a_pin.read();
	const channel_b = channel_b_pin.read();
	const phase_now = Number(channel_a) << 1 | Number(channel_b);
	print('phase ' + PHASE_LABELS[phase_now]);
	if (phase_last !== PHASE_NONE) {
		switch (phase_last << 2 | phase_now) {
		case PHASE_A << 2 | PHASE A: // nichts passiert!
			break;
		case PHASE_A << 2 | PHASE B: // vorwärts
			counter++;
			break;
		case PHASE_A << 2 | PHASE C: // Fehler!
			break mainloop;
		case PHASE_A << 2 | PHASE D: // rückwärts
		case PHASE_B << 2 | PHASE A: // rückwärts
			counter--;
			break;
		case PHASE_B << 2 | PHASE B: // nichts passiert!
			break;
		case PHASE_B << 2 | PHASE C: // vorwärts
			counter++;
			break;
		case PHASE_B << 2 | PHASE D: // Fehler!
		case PHASE_C << 2 | PHASE A: // Fehler!
			break mainloop;
		case PHASE_C << 2 | PHASE B: // rückwärts
			counter--;
			break;
		case PHASE_C << 2 | PHASE C: // nichts passiert!
			break;
		case PHASE_C << 2 | PHASE D: // vorwärts
		case PHASE_D << 2 | PHASE A: // vorwärts
			counter++;
			break;
		case PHASE_D << 2 | PHASE B: // Fehler!
			break mainloop;
		case PHASE_D << 2 | PHASE C: // rückwärts
			counter--;
			break;
		case PHASE_D << 2 | PHASE D: // nichts passiert!
			break;
		default: // Fehler!
			break mainloop;
		}
		if (phase_now !== phase_last) {
			print(counter);
		}
		phase_last = phase_now;
	}
	sleep(10);
}

error('Fehler!!!');
