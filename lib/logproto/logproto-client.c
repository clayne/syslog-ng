/*
 * Copyright (c) 2002-2012 Balabit
 * Copyright (c) 1998-2012 Balázs Scheidler
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */


#include "logproto-client.h"
#include "messages.h"
#include "cfg.h"
#include "plugin.h"
#include "plugin-types.h"

gboolean
log_proto_client_validate_options_method(LogProtoClient *s)
{
  return TRUE;
}

void
log_proto_client_free_method(LogProtoClient *s)
{
  log_transport_stack_shutdown(&s->transport_stack);
  log_transport_stack_deinit(&s->transport_stack);
}

void
log_proto_client_free(LogProtoClient *s)
{
  if (s->free_fn)
    s->free_fn(s);
  g_free(s);
}

void
log_proto_client_init(LogProtoClient *self, LogTransport *transport, const LogProtoClientOptionsStorage *options)
{
  self->validate_options = log_proto_client_validate_options_method;
  self->free_fn = log_proto_client_free_method;
  self->options = options;
  log_transport_stack_init(&self->transport_stack, transport);
}

void
log_proto_client_options_set_drop_input(LogProtoClientOptionsStorage *options, gboolean drop_input)
{
  options->super.drop_input = drop_input;
}

void
log_proto_client_options_set_timeout(LogProtoClientOptionsStorage *options, gint timeout)
{
  options->super.idle_timeout = timeout;
}

gint
log_proto_client_options_get_timeout(LogProtoClientOptionsStorage *options)
{
  return options->super.idle_timeout;
}

void
log_proto_client_options_defaults(LogProtoClientOptionsStorage *options)
{
  options->super.drop_input = FALSE;
  options->super.idle_timeout = 0;
}

void
log_proto_client_options_init(LogProtoClientOptionsStorage *options, GlobalConfig *cfg)
{
}

void
log_proto_client_options_destroy(LogProtoClientOptionsStorage *options)
{
}

LogProtoClientFactory *
log_proto_client_get_factory(PluginContext *context, const gchar *name)
{
  Plugin *plugin;

  plugin = plugin_find(context, LL_CONTEXT_CLIENT_PROTO, name);
  if (plugin && plugin->construct)
    return plugin->construct(plugin);

  return NULL;
}
