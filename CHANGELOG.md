Changelog

## [Unreleased]

## [1.6.2] - 2025-1-4
### Changed
- Changed the event time to every minute for the firmware update for remote development purposes.

## [1.6.1] - 2024-08-09
### Changed
- Changed the event time to every hour for the firmware update for debugging purposes.
- Increased the IMU acceleration threshold from 0.03 to 0.1

## [1.6.0] - 2024-11-15
### Changed
- Initial version of firmware for long term testing/deployment


## [1.2.1] - 2024-08-09
### Changed
- Changed the event time to 3:00 AM for the firmware update.

## [1.2.0] - 2024-08-09
### Added
- Initial implementation of firmware update functionality.
- Added JWT-based authentication for firmware updates.
- Implemented version checking and OTA update logic.

### Changed
- Refactored `HTTPUpdate::update` to accept better version of server.
- Updated `latestFirmware` function to handle different HTTP response codes.

### Fixed
- Fixed issue with HTTPS connection initialization in `latestFirmware`.
- Corrected version parsing logic in `globals.cpp`.

### TODO
- Need to change the server to the main server when the backend changes are merged.

## [1.1.0] - 2024-08-04 (Check this date)
-TODO: Add changes here

## [1.0.0] - 2024-08-03
### Added
- Initial release with basic firmware update and version checking features.
- Implemented secure WiFi client setup.
- Added LED indicators for firmware update status.