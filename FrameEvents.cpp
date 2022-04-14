#include "Frame.h"

//                             --- Menu events

void Frame::newMatchClicked(wxCommandEvent &) {
	if (m_pcTurn) return;

	if (m_isPlaying) {
		wxMessageDialog dialog(this, strings["game.leave"], strings["game.new"], wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
		m_isPlaying = false;
	}

	m_isEnd = false;

	updateChessboard();
	updateStatusText();

	Refresh();
}

void Frame::closeFrame(wxCommandEvent &) {
	Close();
}

void Frame::changeDifficultClicked(wxCommandEvent &) {
	if (m_pcTurn) {
		wxMessageDialog dialog(this, strings["game.wait.text"], strings["game.wait"]);
		dialog.ShowModal();
		return;
	}

	if (m_isPlaying) {
		wxMessageDialog dialog(this, strings["game.leave"], strings["game.new"], wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
	}

	while (true) {
		wxTextEntryDialog dialog(this, strings["game.diff.text"], strings["game.diff"]);
		if (dialog.ShowModal() != wxID_OK) return;
		auto strValue = dialog.GetValue();
		long value;
		if (strValue.ToLong(&value) && value >= minGD && value <= maxGD) {
			m_isPlaying = false;
			m_isEnd = false;
			gameDifficult = (int) value;

			updateChessboard();
			updateStatusText();

			Refresh();
			return;
		}
	}
}

void Frame::aboutClicked(wxCommandEvent &) {
	wxAboutDialogInfo dialog;
	dialog.SetName(strings.get("app.title", PROJECT_NAME));
	dialog.SetVersion(PROJECT_VERSION);
	dialog.SetDescription(strings["app.about.desc"]);
	dialog.SetCopyright(strings["app.about.copy"]);
	dialog.SetDevelopers(developers);
	wxAboutBox(dialog, this);
}

//                             --- Other events

void Frame::OnItemMouseClicked(wxMouseEvent &event) {
	if (m_isEnd) return;
	if (m_pcTurn) {
		wxMessageDialog dialog(this, strings["game.wait.text"], strings["game.wait"]);
		dialog.ShowModal();
		return;
	}
	int x, y;
	event.GetPosition(&x, &y);
	if (x < 0 || y < 0 || x >= squareSize || y >= squareSize) return; // illegal position

	int currentPos = event.GetId();

	if (selectedPos == selectedNone) {
		checkUpdateSelection(currentPos);
		return;
	}

	// already selected

	// same square or white square, illegal move
	if (currentPos == selectedPos || (currentPos / CHESSBOARD_SIZE) % 2 != currentPos % 2) {
		checkUpdateSelection(); // it removes also the possible move borders
		return; // no move
	}

	Chessboard::PieceType value = board.get(currentPos);
	if (value != Chessboard::EMPTY) {
		checkUpdateSelection(currentPos);
		return; // no move
	}

	int oldSelection = selectedPos;
	checkUpdateSelection();
	Chessboard::Move *playerMove = findPlayerMove(oldSelection, currentPos);
	if (playerMove == nullptr) {
		Refresh();
		return; // illegal move
	}

	updateBoardAndIcons(playerMove);
	m_isPlaying = true;
	m_pcTurn = true;

	updateStatusText();
	auto *thread = new MinimaxThread(this, board, gameDifficult, THREAD_FINISH);
	if (thread->Create() != wxTHREAD_NO_ERROR || thread->Run() != wxTHREAD_NO_ERROR) {
		std::cout << "Thread error\n";
		exit(-1);
	}

	// The player move is in the moves vector that will be deleted after the PC move
	Refresh();
}

void Frame::onThreadFinished(wxCommandEvent &event) {
	m_pcTurn = false;
	auto *pcMove = reinterpret_cast<Chessboard::Move *>(event.GetClientData());
	if (pcMove == nullptr) {
		m_isEnd = true;
		m_isPlaying = false;
		updateStatusText(strings["game.you_won"]);
		wxMessageDialog dialog(this, strings["game.you_won"],
		                       strings["game.over"]);
		dialog.ShowModal();
		return;
	}

	updateChessboard(pcMove);

	if (moves.empty()) {
		m_isEnd = true;
		m_isPlaying = false;
		updateStatusText(strings["game.you_lost"]);
		wxMessageDialog dialog(this, strings["game.you_lost"],
		                       strings["game.over"]);
		dialog.ShowModal();
	} else {
		updateStatusText();
	}
}
