.. _sensor:

Sensors
#######

The sensor subsystem exposes an API to uniformly read, configure, and setup event
handling for devices that take real world measurements in meaningful units.

Sensors range from very simple temperature reading devices that must be polled
with a fixed scale to complex devices taking in readings from multitudes of
sensors and themselves producing new inferred sensor data such as step counts,
presence detection, orientation, and more.

Supporting this wide breadth of devices is a demanding task and the sensor API
attempts to provide a uniform interface to them.


.. _sensor-using:

Using Sensors
*************

Using the sensor subsystem from an application there are some APIs and terms that
are helpful to understand. Sensors in Zephyr are composed of :ref:`sensor-channel`,
:ref:`sensor-attribute`, and :ref:`sensor-trigger`. Attributes and triggers may be
device or channel specific.

.. note::
   Getting samples from sensors using the sensor API today can be done in one of
   two ways. A stable and long lived API :ref:`sensor-fetch-and-get` or a newer
   but rapidly stabilizing API :ref:`sensor-read-and-decode`. It's expected that
   in the near future that :ref:`sensor-fetch-and-get` will be deprecated in
   favor of :ref:`sensor-read-and-decode`. Triggers are handled entirely
   differently for :ref:`sensor-fetch-and-get` or :ref:`sensor-read-and-decode`
   and the differences are noted in each of those sections.

.. toctree::
   :maxdepth: 1

   attributes.rst
   channels.rst
   triggers.rst
   power_management.rst
   device_tree.rst
   fetch_and_get.rst
   read_and_decode.rst


.. _sensor-implementing:

Implementing Sensor Drivers
***************************

Implementing the driver side of the Sensor API requires an understanding first of
how the sensor APIs are used by reading through :ref:`sensor-using` so do start
there!

Implementing Attributes
=======================

Attributes provide a flexible get/set API pair for mutating and accessing device
attributes.

At a minimum most devices *should* provide the ability to adjust the scale,
sampling rate for channels.

Implementing Fetch and Get
==========================

Implementation of :c:type:`sensor_sample_fetch_t` *should* be a blocking call
that stashes the  specified channels (or all sensor channels) as part of the
driver data.

Implementation of :c:type:`sensor_channel_get_t` *should* simply access and
return those stashed values. Implementations *should not* mutate the device or
driver data.

Implementation of :c:type:`sensor_trigger_set_t` *should* store the given address
of the trigger rather than copying it. Copying it would prevent usage of the
CONTAINER_OF macro which is the method to provide context around a trigger
when using this API.

Implementing Read and Decode
============================

Implementing the driver for read and decode the driver requires implementing
:c:type:`sensor_submit_t`. The implementation *should* verify the requested
channels are valid. The implementation *should* verify the provided buffer is big
enough to read the desired data into. The implementation *should* if possible
read directly into the provided buffer. Lastly the implementation *should not*
block the caller in a way that would prevent the calling context from continuing
execution while the read is occurring. Non-blocking behavior may be possible
using RTIO capable bus drivers or work queues if blocking calls are required.

RTIO should be the preferred choice as low latency and low overhead are desired
attributes!

The driver *must* provide an associated :c:type:`sensor_decoder_api` which
provides a stateless decoder to convert the raw sensor data provided by
:c:type:`sensor_submit_t` into useful fixed point values.

.. _sensor-api-reference:

API Reference
***************

.. doxygengroup:: sensor_interface
.. doxygengroup:: sensor_emulator_backend
